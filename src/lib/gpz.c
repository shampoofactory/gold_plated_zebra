#include "gpz.h"

static const uint64_t MSG_LEN_MAX = UINT64_MAX / 8;

void GPZ_sha1_init(GPZ_SHA1 *ctx)
{
    memset((void *)ctx->buf, 0, 0x40);
    ctx->msg_len = 0;
    ctx->hash[0] = 0x01234567;
    ctx->hash[1] = 0x89ABCDEF;
    ctx->hash[2] = 0xFEDCBA98;
    ctx->hash[3] = 0x76543210;
    ctx->hash[4] = 0xF0E1D2C3;
}

void GPZ_sha1_wipe(GPZ_SHA1 *ctx)
{
    memset((void *)ctx->buf, 0, sizeof(GPZ_SHA1));
}

bool GPZ_sha1_update(GPZ_SHA1 *ctx, const uint8_t *src, uint64_t src_len)
{
    if (MSG_LEN_MAX - ctx->msg_len < src_len)
    {
        return false;
    }
    uint64_t buf_off = ctx->msg_len & 0x3F;
    if (buf_off)
    {
        uint64_t buf_remain_len = 0x40 - buf_off;
        if (src_len < buf_remain_len)
        {
            memcpy(ctx->buf + buf_off, src, src_len);
            ctx->msg_len += src_len;
            return true;
        }
        memcpy(ctx->buf + buf_off, src, buf_remain_len);
        _sha1_transform((uint8_t *)ctx->hash, ctx->buf, 1);
        ctx->msg_len += buf_remain_len;
        src += buf_remain_len;
        src_len -= buf_remain_len;
    }
    uint64_t n_blocks = src_len / 0x40;
    uint64_t n_blocks_len = src_len & (-0x40);
    _sha1_transform((uint8_t *)ctx->hash, src, n_blocks);
    ctx->msg_len += src_len;
    src += n_blocks_len;
    src_len -= n_blocks_len;
    memcpy(ctx->buf, src, src_len);
    return true;
}

void GPZ_sha1_digest(GPZ_SHA1 *ctx, uint8_t digest[20])
{
    uint32_t buf_off = ctx->msg_len & 0x3F;
    memset(ctx->buf + buf_off, 0, 0x40 - buf_off);
    ctx->buf[buf_off] = 0x80;
    if (buf_off >= 0x38)
    {
        _sha1_transform((uint8_t *)ctx->hash, ctx->buf, 1);
        memset(ctx->buf, 0, 0x38);
    }
    _sha1_store_len(ctx->buf, ctx->msg_len * 8);
    _sha1_transform((uint8_t *)ctx->hash, ctx->buf, 1);
    memcpy((void *)digest, ctx->hash, 20);
    memset(ctx->hash, 0, 20);
}
