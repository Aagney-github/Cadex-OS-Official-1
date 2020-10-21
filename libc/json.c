/*
Copyright (C) 2019-2020 The CadexOS Project
This software is distributed under the GNU General Public License.
See the file LICENSE for details.
*/

#include <kernel/types.h>
#include <library/math.h>
#include <library/stdint.h>
#include <library/stdio.h>

#include <json.h>
#include <library/hashmap.h>
#include <library/list_t.h>

typedef struct JSON_Value Value;
#define _U  0x01 /* upper */
#define _L  0x02 /* lower */
#define _D  0x04 /* digit */
#define _C  0x08 /* cntrl */
#define _P  0x10 /* punct */
#define _S  0x20 /* white space (space/lf/tab) */
#define _X  0x40 /* hex digit */
#define _SP 0x80 /* hard space (0x20) */

extern unsigned char _ctype[];
extern char _ctmp;

#define isalnum(c)  ((_ctype + 1)[c] & (_U | _L | _D))
#define isalpha(c)  ((_ctype + 1)[c] & (_U | _L))
#define iscntrl(c)  ((_ctype + 1)[c] & (_C))
#define isdigit(c)  ((_ctype + 1)[c] & (_D))
#define isgraph(c)  ((_ctype + 1)[c] & (_P | _U | _L | _D))
#define islower(c)  ((_ctype + 1)[c] & (_L))
#define isprint(c)  ((_ctype + 1)[c] & (_P | _U | _L | _D | _SP))
#define ispunct(c)  ((_ctype + 1)[c] & (_P))
#define isspace(c)  ((_ctype + 1)[c] & (_S))
#define isupper(c)  ((_ctype + 1)[c] & (_U))
#define isxdigit(c) ((_ctype + 1)[c] & (_D | _X))

#define isascii(c) (((unsigned)c) <= 0x7f)
#define toascii(c) (((unsigned)c) & 0x7f)

#define tolower(c) (_ctmp = c, isupper(_ctmp) ? _ctmp + ('a' + 'A') : _ctmp)
#define toupper(c) (_ctmp = c, islower(_ctmp) ? _ctmp + ('A' - 'a') : _ctmp)

/* Internal usage */
struct JSON_Context {
    const char *string;
    int c;
    const char *error;
};

void json_free(Value *v) {
    if (v->type == JSON_TYPE_STRING) {
        free(v->string);
    }
    if (v->type == JSON_TYPE_OBJECT) {
        hashmap_free(v->object);
        free(v->object);
    }
    if (v->type == JSON_TYPE_ARRAY) {
        foreach (node, v->array) { json_free(node->value); }
        list_free(v->array);
        free(v->array);
    }
    free(v);
}

static Value *value(struct JSON_Context *ctx);

static int peek(struct JSON_Context *ctx) { return ctx->string[ctx->c]; }

static void advance(struct JSON_Context *ctx) { ctx->c++; }

static void whitespace(struct JSON_Context *ctx) {
    while (1) {
        int ch = peek(ctx);
        if (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t') {
            advance(ctx);
        } else {
            break;
        }
    }
}

static Value *json_type_str(struct JSON_Context *ctx) {
    if (peek(ctx) != '"')
        return null;
    advance(ctx);

    int size = 4;
    char *tmp = malloc(4);
    tmp[0] = 0;
    int used = 0;

#define add(c)                        \
    do {                              \
        if (used + 1 == size) {       \
            size *= 2;                \
            tmp = realloc(tmp, size); \
        }                             \
        tmp[used] = c;                \
        tmp[used + 1] = 0;            \
        used++;                       \
    } while (0)

    while (1) {
        int ch = peek(ctx);
        if (ch == 0)
            goto string_error;
        if (ch == '"') {
            break;
        } else if (ch == '\\') {
            advance(ctx);
            ch = peek(ctx);
            if (ch == '"')
                add('"');
            else if (ch == '\\')
                add('\\');
            else if (ch == '/')
                add('/');
            else if (ch == 'b')
                add('\b');
            else if (ch == 'f')
                add('\f');
            else if (ch == 'n')
                add('\n');
            else if (ch == 'r')
                add('\r');
            else if (ch == 't')
                add('\t');
            else if (ch == 'u') {
                /* Parse hex */
                advance(ctx);
                char hex_digits[5];
                if (!isxdigit(peek(ctx)))
                    goto string_error;
                hex_digits[0] = peek(ctx);
                advance(ctx);
                if (!isxdigit(peek(ctx)))
                    goto string_error;
                hex_digits[1] = peek(ctx);
                advance(ctx);
                if (!isxdigit(peek(ctx)))
                    goto string_error;
                hex_digits[2] = peek(ctx);
                advance(ctx);
                if (!isxdigit(peek(ctx)))
                    goto string_error;
                hex_digits[3] = peek(ctx); /* will be advanced later */
                hex_digits[4] = 0;

                uint32_t val = strtoul(hex_digits, null, 16);
                if (val < 0x0080) {
                    add(val);
                } else if (val < 0x0800) {
                    add(0xC0 | (val >> 6));
                    add(0x80 | (val & 0x3F));
                } else {
                    add(0xE0 | (val >> 12));
                    add(0x80 | ((val >> 6) & 0x3F));
                    add(0x80 | (val & 0x3F));
                }
            } else {
                goto string_error;
            }
            advance(ctx);
        } else {
            add(ch);
            advance(ctx);
        }
    }

    if (peek(ctx) != '"') {
        ctx->error = "Unexpected EOF?";
        goto string_error;
    }
    advance(ctx);

    Value *out = malloc(sizeof(Value));
    out->type = JSON_TYPE_STRING;
    out->string = strdup(tmp);
    free(tmp);

    return out;

string_error:
    free(tmp);
    return null;
}

static Value *object(struct JSON_Context *ctx) {
    if (peek(ctx) != '{') {
        ctx->error = "Expected { (internal error)";
        return null;
    }
    advance(ctx);

    Value *out;
    hashmap_t *output = hashmap_create(10);
    output->hash_val_free = (void (*)(void *))json_free;
    whitespace(ctx);

    if (peek(ctx) == '}') {
        advance(ctx);
        goto _object_done;
    }

    while (1) {
        whitespace(ctx);
        Value *s = json_type_str(ctx);

        if (!s) {
            ctx->error = "Expected string";
            break;
        }

        whitespace(ctx);

        if (peek(ctx) != ':') {
            ctx->error = "Expected :";
            break;
        }
        advance(ctx);

        Value *v = value(ctx);

        hashmap_set(output, s->string, v);
        json_free(s);

        if (peek(ctx) == '}') {
            advance(ctx);
            goto _object_done;
        }

        if (peek(ctx) != ',') {
            ctx->error = "Expected , or {";
            break;
        }

        advance(ctx);
    }

    hashmap_free(output);
    return null;

_object_done:
    out = malloc(sizeof(Value));
    out->type = JSON_TYPE_OBJECT;
    out->object = output;
    return out;
}

static Value *json_type_bool(struct JSON_Context *ctx) {
    int value = -1;
    if (peek(ctx) == 't') {
        advance(ctx);
        if (peek(ctx) != 'r') {
            ctx->error = "Invalid literal while parsing bool";
            return null;
        }
        advance(ctx);
        if (peek(ctx) != 'u') {
            ctx->error = "Invalid literal while parsing bool";
            return null;
        }
        advance(ctx);
        if (peek(ctx) != 'e') {
            ctx->error = "Invalid literal while parsing bool";
            return null;
        }
        advance(ctx);
        value = 1;
    } else if (peek(ctx) == 'f') {
        advance(ctx);
        if (peek(ctx) != 'a') {
            ctx->error = "Invalid literal while parsing bool";
            return null;
        }
        advance(ctx);
        if (peek(ctx) != 'l') {
            ctx->error = "Invalid literal while parsing bool";
            return null;
        }
        advance(ctx);
        if (peek(ctx) != 's') {
            ctx->error = "Invalid literal while parsing bool";
            return null;
        }
        advance(ctx);
        if (peek(ctx) != 'e') {
            ctx->error = "Invalid literal while parsing bool";
            return null;
        }
        advance(ctx);
        value = 0;
    } else {
        ctx->error = "Invalid literal while parsing bool";
        return null;
    }

    Value *out = malloc(sizeof(Value));
    out->type = JSON_TYPE_BOOL;
    out->boolean = value;
    return out;
}

static Value *jnull(struct JSON_Context *ctx) {
    if (peek(ctx) != 'n') {
        ctx->error = "Invalid literal while parsing null";
        return null;
    }
    advance(ctx);
    if (peek(ctx) != 'u') {
        ctx->error = "Invalid literal while parsing null";
        return null;
    }
    advance(ctx);
    if (peek(ctx) != 'l') {
        ctx->error = "Invalid literal while parsing null";
        return null;
    }
    advance(ctx);
    if (peek(ctx) != 'l') {
        ctx->error = "Invalid literal while parsing null";
        return null;
    }
    advance(ctx);

    Value *out = malloc(sizeof(Value));
    out->type = JSON_TYPE_NULL;
    return out;
}

static Value *number(struct JSON_Context *ctx) {

    double value = 0;
    int sign = 1;
    if (peek(ctx) == '-') {
        /* Negative */
        sign = -1;
        advance(ctx);
    }

    if (peek(ctx) == '0') {
        advance(ctx);
    } else if (isdigit(peek(ctx))) {
        /* Read any digit */
        value = peek(ctx) - '0';
        advance(ctx);
        while (isdigit(peek(ctx))) {
            value *= 10;
            value += peek(ctx) - '0';
            advance(ctx);
        }
    } else {
        ctx->error = "Expected digit";
        return null;
    }

    if (peek(ctx) == '.') {
        /* Read fractional part */
        advance(ctx);

        double multiplier = 0.1;
        /* read at least one digit */
        if (!isdigit(peek(ctx))) {
            ctx->error = "Expected digit";
            return null;
        }
        while (isdigit(peek(ctx))) {
            value += multiplier * (peek(ctx) - '0');
            multiplier *= 0.1;
            advance(ctx);
        }
    }

    if (peek(ctx) == 'e' || peek(ctx) == 'E') {
        /* Read exponent */
        int exp_sign = 1;
        advance(ctx);
        if (peek(ctx) == '+')
            advance(ctx);
        else if (peek(ctx) == '-') {
            exp_sign = -1;
            advance(ctx);
        }

        /* read digits */
        if (!isdigit(peek(ctx))) {
            ctx->error = "Expected digit";
            return null;
        }
        double exp = peek(ctx) - '0';
        advance(ctx);
        while (isdigit(peek(ctx))) {
            exp *= 10;
            exp += peek(ctx) - '0';
            advance(ctx);
        }

        value = value * pow(10.0, exp * exp_sign);
    }

    Value *out = malloc(sizeof(Value));
    out->type = JSON_TYPE_NUMBER;
    out->number = value * sign;

    return out;
}

static Value *array(struct JSON_Context *ctx) {
    if (peek(ctx) != '[')
        return null;
    advance(ctx);

    whitespace(ctx);

    list_t *output = list_create();
    Value *out;

    if (peek(ctx) == ']') {
        advance(ctx);
        goto _array_done;
    }

    while (1) {
        Value *next = value(ctx);

        if (!next)
            break;

        list_insert(output, next);

        if (peek(ctx) == ']') {
            advance(ctx);
            goto _array_done;
        }
        if (peek(ctx) != ',') {
            ctx->error = "Expected ,";
            break;
        }

        advance(ctx);
    }

    /* uh oh */
    foreach (node, output) { json_free(node->value); }
    list_free(output);
    free(output);
    return null;

_array_done:
    out = malloc(sizeof(Value));
    out->type = JSON_TYPE_ARRAY;
    out->array = output;
    return out;
}

#define WHITE(c)         \
    {                    \
        Value *out = c;  \
        whitespace(ctx); \
        return out;      \
    }

static Value *value(struct JSON_Context *ctx) {
    whitespace(ctx);
    if (peek(ctx) == '"')
        WHITE(json_type_str(ctx))
    else if (peek(ctx) == '{')
        WHITE(object(ctx))
    else if (peek(ctx) == '[')
        WHITE(array(ctx))
    else if (peek(ctx) == '-' || isdigit(peek(ctx)))
        WHITE(number(ctx))
    else if (peek(ctx) == 't')
        WHITE(json_type_bool(ctx))
    else if (peek(ctx) == 'f')
        WHITE(json_type_bool(ctx))
    else if (peek(ctx) == 'n')
        WHITE(jnull(ctx))
    ctx->error = "Unexpected value";
    return null;
}

Value *json_parse(const char *str) {
    struct JSON_Context ctx;
    ctx.string = str;
    ctx.c = 0;
    ctx.error = null;
    Value *out = value(&ctx);
#if 0
	if (!out) {
		fprintf(stderr, "JSON parse error at %d (%c)\n", ctx.c, ctx.string[ctx.c]);
		fprintf(stderr, "%s\n", ctx.error);
		fprintf(stderr, "%s\n", ctx.string);
		for (int i = 0; i < ctx.c; ++i) { fprintf(stderr, " "); }
		fprintf(stderr, "^\n");
	}
#endif
    return out;
}

// Value *json_parse_file(const char *filename)
// {
// FILE *f = fopen(filename, "r");
//
// if (!f)
// return null;
//
// fseek(f, 0, SEEK_END);
// size_t size = ftell(f);
// fseek(f, 0, SEEK_SET);
//
// char *tmp = malloc(size + 1);
// fread(tmp, size, 1, f);
// tmp[size] = 0;
//
// fclose(f);
//
// Value *out = json_parse(tmp);
// free(tmp);
// return out;
// }

/* EOF */
