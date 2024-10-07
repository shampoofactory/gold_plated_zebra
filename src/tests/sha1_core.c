#include "sha1_core.h"

static bool dec_val(Bytes src, uint64_t *dst)
{
    if (bytes_parse_uint64(src, dst))
    {
        return true;
    }
    fprintf(stderr, "SHA1Core: bad decimal value: ");
    bytes_fprintln_chr(src, stderr);
    return false;
}

static bool hex_val(Bytes src, Bytes *dst, size_t dst_len)
{
    dst->len = dst_len;
    if (bytes_parse_hex(src, dst))
    {
        return true;
    }
    fprintf(stderr, "SHA1Core: bad hex value: ");
    bytes_fprintln_chr(src, stderr);
    return false;
}

static bool sha1_core_tag(RSPCore *ctx, Bytes tag)
{
    return true;
}

static bool sha1_core_key_val(RSPCore *ctx, Bytes key, Bytes val)
{
    ((SHA1Core *)ctx)->is_set = true;
    if (bytes_equal_string(key, "Len"))
    {
        return dec_val(val, &((SHA1Core *)ctx)->len);
    }
    if (bytes_equal_string(key, "Msg"))
    {
        return hex_val(val, &((SHA1Core *)ctx)->msg, MSG_LEN);
    }
    if (bytes_equal_string(key, "MD"))
    {
        return hex_val(val, &((SHA1Core *)ctx)->md, MD_LEN);
    }
    fprintf(stderr, "SHA1Core: unknown key: ");
    bytes_fprintln_chr(key, stderr);
    return false;
}

static bool sha1_core_0_update(SHA1Core *ctx, GPZ_SHA1 *sha1_ctx)
{
    return GPZ_sha1_update(sha1_ctx, ctx->msg.ptr, ctx->len / 8);
}

static bool sha1_core_1_update(SHA1Core *ctx, GPZ_SHA1 *sha1_ctx)
{
    for (size_t i = 0; i < ctx->len / 8; i++)
    {
        if (!GPZ_sha1_update(sha1_ctx, ctx->msg.ptr + i, 1))
        {
            return false;
        }
    }
    return true;
}

static bool sha1_core_2_update(SHA1Core *ctx, GPZ_SHA1 *sha1_ctx)
{
    if (!ctx->len)
    {
        return true;
    }
    return GPZ_sha1_update(sha1_ctx, ctx->msg.ptr, 1) &&
           GPZ_sha1_update(sha1_ctx, ctx->msg.ptr, 0) &&
           GPZ_sha1_update(sha1_ctx, ctx->msg.ptr + 1, ctx->len / 8 - 1);
}

static bool sha1_core_3_update(SHA1Core *ctx, GPZ_SHA1 *sha1_ctx)
{
    if (ctx->len < 64)
    {
        return GPZ_sha1_update(sha1_ctx, ctx->msg.ptr, ctx->len / 8);
    }
    return GPZ_sha1_update(sha1_ctx, ctx->msg.ptr, 64) &&
           GPZ_sha1_update(sha1_ctx, ctx->msg.ptr, 0) &&
           GPZ_sha1_update(sha1_ctx, ctx->msg.ptr + 64, ctx->len / 8 - 64);
}

static bool sha1_core_go(RSPCore *ctx)
{
    if (!((SHA1Core *)ctx)->is_set)
    {
        return true;
    }
    ((SHA1Core *)ctx)->is_set = false;
    if (((SHA1Core *)ctx)->len > MSG_LEN)
    {
        fprintf(stderr, "SHA1Core: Len overflow: %lu\n", ((SHA1Core *)ctx)->len);
        return false;
    }
    uint8_t store[20] = {};
    Bytes md = bytes_with(store, 20);
    GPZ_SHA1 sha1_ctx;
    GPZ_sha1_init(&sha1_ctx);
    if (((SHA1Core *)ctx)->update((SHA1Core *)ctx, &sha1_ctx))
    {
        GPZ_sha1_digest(&sha1_ctx, md.ptr);
        if (bytes_equal(((SHA1Core *)ctx)->md, md))
        {
            return true;
        }
    }
    fprintf(stderr, "SHA1Core: FAIL:\n");
    fprintf(stderr, "Len: %lu\n", ((SHA1Core *)ctx)->len);
    fprintf(stderr, "Msg: ");
    bytes_fprintln_hex(((SHA1Core *)ctx)->msg, stderr);
    fprintf(stderr, "MD<: ");
    bytes_fprintln_hex(((SHA1Core *)ctx)->md, stderr);
    fprintf(stderr, "MD>: ");
    bytes_fprintln_hex(md, stderr);
    return false;
}

static SHA1Core sha1_core_build(bool (*update)(SHA1Core *, GPZ_SHA1 *))
{
    uint8_t *byte_store = (uint8_t *)malloc(BLOCK_LEN);
    if (byte_store == NULL)
    {
        fprintf(stderr, "SHA1Core: create: malloc NULL\n");
        exit(1);
    }
    return (SHA1Core){
        .core = (RSPCore){
            .tag = sha1_core_tag,
            .key_val = sha1_core_key_val,
            .go = sha1_core_go,
        },
        .is_set = false,
        .update = update,
        .byte_store = byte_store,
        .len = 0,
        .md = bytes_with(byte_store + MD_OFF, 0),
        .msg = bytes_with(byte_store + MSG_OFF, 0),
    };
}

SHA1Core sha1_core_0_create()
{
    return sha1_core_build(sha1_core_0_update);
}

SHA1Core sha1_core_1_create()
{
    return sha1_core_build(sha1_core_1_update);
}

SHA1Core sha1_core_2_create()
{
    return sha1_core_build(sha1_core_2_update);
}

SHA1Core sha1_core_3_create()
{
    return sha1_core_build(sha1_core_3_update);
}

void sha1_destroy(SHA1Core *ctx)
{
    free(ctx->byte_store);
    ctx->is_set = false;
    ctx->len = 0;
    ctx->md = bytes_null();
    ctx->msg = bytes_null();
    ctx->byte_store = NULL;
}
