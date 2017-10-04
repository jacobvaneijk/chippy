/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include <check.h>
#include <libchippy/chippy.h>
#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>

static struct chippy *chippy_create(void) {
    struct chippy *machine = malloc(sizeof(struct chippy));

    chippy_init(machine);

    return machine;
}

static void chippy_insert_opcode(struct chippy *machine, uint16_t opcode, uint16_t address) {
    machine->ram[address] = opcode >> 8;
    machine->ram[address + 1] = opcode & 0xFF;
}

START_TEST(test_cls)
{
    struct chippy *machine = chippy_create();

    memset(machine->gfx, 1, SCREEN_W * SCREEN_H);

    chippy_insert_opcode(machine, 0x00E0, 0x200);
    chippy_step(machine);

    for (int i = 0; i < (SCREEN_W * SCREEN_H); i++) {
        ck_assert_int_eq(machine->gfx[i], 0);
    }
}
END_TEST

START_TEST(test_ret)
{
    struct chippy *machine = chippy_create();

    machine->stack[machine->sp++] = 0x210;
    machine->stack[machine->sp++] = 0x220;
    machine->stack[machine->sp++] = 0x230;

    ck_assert_int_eq(machine->sp, 3);

    chippy_insert_opcode(machine, 0x00EE, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->pc, 0x230);
    ck_assert_int_eq(machine->sp, 2);
}
END_TEST

START_TEST(test_jp)
{
    struct chippy *machine = chippy_create();

    chippy_insert_opcode(machine, 0x1234, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->pc, 0x234);
}
END_TEST

START_TEST(test_call)
{
    struct chippy *machine = chippy_create();

    machine->stack[machine->sp++] = 0x234;

    ck_assert_int_eq(machine->sp, 1);

    chippy_insert_opcode(machine, 0x2345, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->sp, 2);
    ck_assert_int_eq(machine->pc, 0x345);
}
END_TEST

START_TEST(test_se_xkk)
{
    struct chippy *machine = chippy_create();

    machine->V[2] = 0x12;

    chippy_insert_opcode(machine, 0x3212, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->pc, 0x204);
}
END_TEST

START_TEST(test_sne_xkk)
{
    struct chippy *machine = chippy_create();

    machine->V[2] = 0x12;

    chippy_insert_opcode(machine, 0x4299, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->pc, 0x204);
}
END_TEST

START_TEST(test_se_xy)
{
    struct chippy *machine = chippy_create();

    machine->V[2] = 0x12;
    machine->V[3] = 0x12;

    chippy_insert_opcode(machine, 0x5230, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->pc, 0x204);
}
END_TEST

START_TEST(test_ld_kk)
{
    struct chippy *machine = chippy_create();

    chippy_insert_opcode(machine, 0x6230, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[2], 0x30);
}
END_TEST

START_TEST(test_add_xkk)
{
    struct chippy *machine = chippy_create();

    chippy_insert_opcode(machine, 0x7120, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 0x20);
}
END_TEST

START_TEST(test_ld_xy)
{
    struct chippy *machine = chippy_create();

    machine->V[2] = 3;

    chippy_insert_opcode(machine, 0x8121, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 3);
}
END_TEST

START_TEST(test_or)
{
    struct chippy *machine = chippy_create();

    machine->V[1] = 5;
    machine->V[2] = 3;

    chippy_insert_opcode(machine, 0x8122, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 7);
}
END_TEST

START_TEST(test_xor)
{
    struct chippy *machine = chippy_create();

    machine->V[1] = 5;
    machine->V[2] = 3;

    chippy_insert_opcode(machine, 0x8123, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 6);
}
END_TEST

START_TEST(test_add_xy)
{
    struct chippy *machine = chippy_create();

    machine->V[1] = 5;
    machine->V[2] = 3;

    chippy_insert_opcode(machine, 0x8124, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 8);
    ck_assert_int_eq(machine->V[0xF], 0);

    machine->V[1] = 200;
    machine->V[2] = 200;

    chippy_insert_opcode(machine, 0x8124, 0x202);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 144);
    ck_assert_int_eq(machine->V[0xF], 1);
}
END_TEST

START_TEST(test_sub_xy)
{
    struct chippy *machine = chippy_create();

    machine->V[1] = 5;
    machine->V[2] = 3;

    chippy_insert_opcode(machine, 0x8125, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 2);
    ck_assert_int_eq(machine->V[0xF], 1);

    machine->V[1] = 3;
    machine->V[2] = 5;

    chippy_insert_opcode(machine, 0x8125, 0x202);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 254);
    ck_assert_int_eq(machine->V[0xF], 0);
}
END_TEST

START_TEST(test_shr)
{
    struct chippy *machine = chippy_create();

    machine->V[1] = 23;

    chippy_insert_opcode(machine, 0x8106, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 11);
    ck_assert_int_eq(machine->V[0xF], 1);

    machine->V[1] = 22;

    chippy_insert_opcode(machine, 0x8106, 0x202);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 11);
    ck_assert_int_eq(machine->V[0xF], 0);
}
END_TEST

START_TEST(test_subn)
{
    struct chippy *machine = chippy_create();

    machine->V[1] = 3;
    machine->V[2] = 5;

    chippy_insert_opcode(machine, 0x8127, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 2);
    ck_assert_int_eq(machine->V[0xF], 1);

    machine->V[1] = 5;
    machine->V[2] = 3;

    chippy_insert_opcode(machine, 0x8127, 0x202);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 254);
    ck_assert_int_eq(machine->V[0xF], 0);
}
END_TEST

START_TEST(test_shl)
{
    struct chippy *machine = chippy_create();

    machine->V[1] = 23;

    chippy_insert_opcode(machine, 0x810E, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 46);
    ck_assert_int_eq(machine->V[0xF], 0);

    machine->V[1] = 151;

    chippy_insert_opcode(machine, 0x810E, 0x202);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 46);
    ck_assert_int_eq(machine->V[0xF], 1);
}

END_TEST
Suite *create_opcodes_suite(void) {
    Suite *suite = suite_create("Opcodes");
    TCase *chain = tcase_create("opcode tests");

    suite_add_tcase(suite, chain);

    tcase_add_test(chain, test_cls);
    tcase_add_test(chain, test_ret);
    tcase_add_test(chain, test_jp);
    tcase_add_test(chain, test_call);
    tcase_add_test(chain, test_se_xkk);
    tcase_add_test(chain, test_sne_xkk);
    tcase_add_test(chain, test_se_xy);
    tcase_add_test(chain, test_ld_kk);
    tcase_add_test(chain, test_add_xkk);
    tcase_add_test(chain, test_ld_xy);
    tcase_add_test(chain, test_or);
    tcase_add_test(chain, test_xor);
    tcase_add_test(chain, test_add_xy);
    tcase_add_test(chain, test_sub_xy);
    tcase_add_test(chain, test_shr);
    tcase_add_test(chain, test_subn);
    tcase_add_test(chain, test_shl);

    return suite;
}
