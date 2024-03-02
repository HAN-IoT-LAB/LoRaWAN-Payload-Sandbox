/* MIT License
 * Copyright (c) 2024 Richard Kroesen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <unity.h>
#include "../include/CayenneLPP.hpp"

void setUp(void)
{
    /* Initialize stuff before each test */
}

void tearDown(void)
{
    /* Clean up after each test */
}

void test_CayenneLPP_Initialization(void) {
    PAYLOAD_ENCODER::CayenneLPP<128> lpp(10);
    TEST_ASSERT_NOT_NULL(lpp.getBuffer());
    TEST_ASSERT_EQUAL_UINT8(0, lpp.getSize());
}

void test_CayenneLPP_Reset(void) {
    PAYLOAD_ENCODER::CayenneLPP<128> lpp(10);
    lpp.addAnalogInput(1, {0.1f});
    lpp.reset();
    TEST_ASSERT_EQUAL_UINT8(0, lpp.getSize());
}

void test_CayenneLPP_Digital(void) {
    PAYLOAD_ENCODER::CayenneLPP<128> lpp(10);
    lpp.addDigitalInput(5, 0xDD);
    TEST_ASSERT_EQUAL_UINT8(3, lpp.getSize());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_CayenneLPP_Initialization);
    RUN_TEST(test_CayenneLPP_Reset);
    RUN_TEST(test_CayenneLPP_Digital);
    UNITY_END();
    return 0;
}
