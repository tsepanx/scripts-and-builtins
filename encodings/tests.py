import unittest
import logging

from bin_funcs import BytesData

func_name = lambda func: func.__code__.co_name
multireverse = lambda arr: [reversed(i) for i in arr]

convert_logger = logging.getLogger('convert')
convert_logger.setLevel(logging.DEBUG)


# hex, bin, int, bytes
class Tests:
    hex_bin = [
        ['8a9b', '1000101010011011'],
        ['1234567890abcdef', '0001001000110100010101100111100010010000101010111100110111101111'],
    ]

    hex_bytes = [
        ['8a9b', b'\x8a\x9b'],
        ['1234567890abcdef', b'\x124Vx\x90\xab\xcd\xef'],
    ]

    hex_int = [
        ['1234567890abcdef', 1311768467294899695],
        # ['1234567890abcde', 81985529205931230],
        # ['1234567890', 78187493520],
    ]


class TestBin(unittest.TestCase):
    def base_test(self, test_func, tests_arr: list):
        def assert_convert(from_value, to_value, convert_func):
            converted = convert_func(from_value)

            print(f'F: {func_name(convert_func)} | {from_value} -> {to_value}')
            convert_logger.info(f'F: {func_name(convert_func)} | {from_value} -> {to_value}')
            return self.assertEqual(converted, to_value)

        for a, b in tests_arr:
            assert_convert(a, b, test_func)

    def _test_both(self, tests_arr, func1, func2):
        tests_arr_reversed = multireverse(tests_arr)

        self.base_test(func1, tests_arr)
        self.base_test(func2, tests_arr_reversed)

    def test_hex_bin(self):
        for i in Tests.hex_bin:
            bin_res = BytesData.from_hex(i[0]).to_bin()
            hex_res = BytesData.from_bin(i[1]).to_hex()

            self.assertEqual(bin_res, i[1])
            self.assertEqual(hex_res, i[0])

    def test_hex_bytes(self):
        for i in Tests.hex_bytes:
            bytes_res = BytesData.from_hex(i[0]).to_bytes()
            hex_res = BytesData.from_bytes(i[1]).to_hex()

            self.assertEqual(bytes_res, i[1])
            self.assertEqual(hex_res, i[0])

    def test_hex_int(self):
        for i in Tests.hex_int:
            int_res = BytesData.from_hex(i[0]).to_int()
            hex_res = BytesData.from_int(i[1]).to_hex()

            self.assertEqual(int_res, i[1])
            self.assertEqual(hex_res, i[0])


if __name__ == '__main__':
    unittest.main()
