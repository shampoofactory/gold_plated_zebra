        default     rel

        global      _sha1_init
        global      _sha1_transform
        global      _sha1_store_len

        section     .data align=0x10

SHUFFLE_CONTROL_MASK:
dq      0x08090A0B0C0D0E0F
dq      0x0001020304050607

INITIAL_HASH_VALUE:
dq      0x89ABCDEF01234567
dq      0x76543210FEDCBA98
dq      0x00000000F0E1D2C3
dq      0x0000000000000000

        section     .text

; HSHP: hash pointer
; MSGP: message block pointer
; MBIT: message bit length
; NBLK: number of 64 byte message blocks

; void *(uint8_t* MSGP, uin64_t MBIT)
align 0x10
_sha1_store_len:
        %define MSGP      rdi
        %define MBIT      rsi
        bswap       MBIT
        mov         [MSGP + 0x38], MBIT
        ret
        %undef MSGP
        %undef MBIT


; void *(uint8_t* HSHP, const uint8_t* MSGP, size_t NBLK)
align 0x10
_sha1_transform:
        %define HSHP      rdi
        %define MSGP      rsi
        %define NBLK      rdx
        %define ABCD      xmm0
        %define E000      xmm1
        %define E001      xmm2
        %define MSG0      xmm3
        %define MSG1      xmm4
        %define MSG2      xmm5
        %define MSG3      xmm6
        %define MASK      xmm7
        %define ABCD_SAVE xmm8
        %define E000_SAVE xmm9
        test        NBLK, NBLK          ; if (NBLK == 0)
        je          .exit               ;   then .exit
        vmovdqa     MASK, [SHUFFLE_CONTROL_MASK]    ; load shuffle mask
        vmovdqu     ABCD, [HSHP]        ; load initial hash value
        vmovd       E000, [HSHP + 0x10]
        vpshufb     ABCD, MASK
        vpshufb     E000, MASK
        jmp         .loop_into
.loop:
        add         MSGP, 0x40          ; next message block
.loop_into:
        vmovdqa     ABCD_SAVE, ABCD     ; save working variables
        vmovdqa     E000_SAVE, E000
        vmovdqu     MSG0, [MSGP]        ; rounds 0 through 3
        vpshufb     MSG0, MASK
        vpaddd      E000, MSG0
        vmovdqa     E001, ABCD
        sha1rnds4   ABCD, E000, 0
        vmovdqu     MSG1, [MSGP + 0x10] ; rounds 4 through 7
        vpshufb     MSG1, MASK
        sha1nexte   E001, MSG1
        vmovdqa     E000, ABCD
        sha1rnds4   ABCD, E001, 0
        sha1msg1    MSG0, MSG1
        vmovdqu     MSG2, [MSGP + 0x20] ; rounds 8 through 11
        vpshufb     MSG2, MASK
        sha1nexte   E000, MSG2
        vmovdqa     E001, ABCD
        sha1rnds4   ABCD, E000, 0
        sha1msg1    MSG1, MSG2
        vpxor       MSG0, MSG2
        vmovdqu     MSG3, [MSGP + 0x30] ; rounds 12 through 15
        vpshufb     MSG3, MASK
        sha1nexte   E001, MSG3
        vmovdqa     E000, ABCD
        sha1msg2    MSG0, MSG3
        sha1rnds4   ABCD, E001, 0
        sha1msg1    MSG2, MSG3
        vpxor       MSG1, MSG3
        sha1nexte   E000, MSG0          ; rounds 16 through 19
        vmovdqa     E001, ABCD
        sha1msg2    MSG1, MSG0
        sha1rnds4   ABCD, E000, 0
        sha1msg1    MSG3, MSG0
        vpxor       MSG2, MSG0
        sha1nexte   E001, MSG1          ; rounds 20 through 23
        vmovdqa     E000, ABCD
        sha1msg2    MSG2, MSG1
        sha1rnds4   ABCD, E001, 1
        sha1msg1    MSG0, MSG1
        vpxor       MSG3, MSG1
        sha1nexte   E000, MSG2          ; rounds 24 through 27
        vmovdqa     E001, ABCD
        sha1msg2    MSG3, MSG2
        sha1rnds4   ABCD, E000, 1
        sha1msg1    MSG1, MSG2
        vpxor       MSG0, MSG2
        sha1nexte   E001, MSG3          ; rounds 28 through 31
        vmovdqa     E000, ABCD
        sha1msg2    MSG0, MSG3
        sha1rnds4   ABCD, E001, 1
        sha1msg1    MSG2, MSG3
        vpxor       MSG1, MSG3
        sha1nexte   E000, MSG0          ; rounds 32 through 35
        vmovdqa     E001, ABCD
        sha1msg2    MSG1, MSG0
        sha1rnds4   ABCD, E000, 1
        sha1msg1    MSG3, MSG0
        vpxor       MSG2, MSG0
        sha1nexte   E001, MSG1          ; rounds 36 through 39
        vmovdqa     E000, ABCD
        sha1msg2    MSG2, MSG1
        sha1rnds4   ABCD, E001, 1
        sha1msg1    MSG0, MSG1
        vpxor       MSG3, MSG1
        sha1nexte   E000, MSG2          ; rounds 40 through 43
        vmovdqa     E001, ABCD
        sha1msg2    MSG3, MSG2
        sha1rnds4   ABCD, E000, 2
        sha1msg1    MSG1, MSG2
        vpxor       MSG0, MSG2
        sha1nexte   E001, MSG3          ; rounds 44 through 47
        vmovdqa     E000, ABCD
        sha1msg2    MSG0, MSG3
        sha1rnds4   ABCD, E001, 2
        sha1msg1    MSG2, MSG3
        vpxor       MSG1, MSG3
        sha1nexte   E000, MSG0          ; rounds 48 through 51
        vmovdqa     E001, ABCD
        sha1msg2    MSG1, MSG0
        sha1rnds4   ABCD, E000, 2
        sha1msg1    MSG3, MSG0
        vpxor       MSG2, MSG0
        sha1nexte   E001, MSG1          ; rounds 52 through 55
        vmovdqa     E000, ABCD
        sha1msg2    MSG2, MSG1
        sha1rnds4   ABCD, E001, 2
        sha1msg1    MSG0, MSG1
        vpxor       MSG3, MSG1
        sha1nexte   E000, MSG2          ; rounds 56 through 59
        vmovdqa     E001, ABCD
        sha1msg2    MSG3, MSG2
        sha1rnds4   ABCD, E000, 2
        sha1msg1    MSG1, MSG2
        vpxor       MSG0, MSG2
        sha1nexte   E001, MSG3          ; rounds 60 through 63
        vmovdqa     E000, ABCD
        sha1msg2    MSG0, MSG3
        sha1rnds4   ABCD, E001, 3
        sha1msg1    MSG2, MSG3
        vpxor       MSG1, MSG3
        sha1nexte   E000, MSG0          ; rounds 64 through 67
        vmovdqa     E001, ABCD
        sha1msg2    MSG1, MSG0
        sha1rnds4   ABCD, E000, 3
        sha1msg1    MSG3, MSG0
        vpxor       MSG2, MSG0
        sha1nexte   E001, MSG1          ; rounds 68 through 71
        vmovdqa     E000, ABCD
        sha1msg2    MSG2, MSG1
        sha1rnds4   ABCD, E001, 3
        vpxor       MSG3, MSG1
        sha1nexte   E000, MSG2          ; rounds 72 through 75
        vmovdqa     E001, ABCD
        sha1msg2    MSG3, MSG2
        sha1rnds4   ABCD, E000, 3
        sha1nexte   E001, MSG3          ; rounds 76 through 79
        vmovdqa     E000, ABCD
        sha1rnds4   ABCD, E001, 3
        sha1nexte   E000, E000_SAVE     ; compute intermediate hash
        vpaddd      ABCD, ABCD_SAVE
        sub         NBLK, 1             ; if (--NBLK != 0)
        jne         .loop               ;   then .loop
        vpshufb     ABCD, MASK
        vpshufb     E000, MASK
        vmovdqu     [HSHP], ABCD        ; store hash value
        vmovd       [HSHP + 0x10], E000
.exit:
        ret
        %undef HSHP
        %undef MSGP
        %undef NBLK
        %undef ABCD
        %undef E000
        %undef E001
        %undef MSG0
        %undef MSG1
        %undef MSG2
        %undef MSG3
        %undef MASK
        %undef ABCD_SAVE
        %undef E000_SAVE
