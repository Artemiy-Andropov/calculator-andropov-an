import subprocess
import pytest

correct_arithmetic_equations = [
    ("1 + 3-2", "2", False),
    ("2 / 4 - 1", "-0.5000", True),
    ("4 * 7 - 6 * 3", "10", False),
    ("32 * 7 / 63 + 41", "44.5556", True),
    ("(4+10) / 3 + (2 * 5 - 1)", "13", False),
    ("(8 * 9 - 67) / (5 - 7 * 6) + 23", "22.8649", True),
    ("7 - 4 +3*(5 + 9*8 - (7+3) / 10) * 2", "459", False),
    ("4 + 5 - (98 / 3 + (21 + 2) / 2) * 5", "-211.8333", True)
]

erroneous_arithmetic_equations = [
    # Incorrect input of numbers/operations
    ("2 * 7 8", "", False),
    ("48 * 9 +7^2", "", False),
    ("a * 7 - 3 + 1", "", False),
    ("++73 - 0 * 6 / 2", "", False),

    # Incorrect input of parentheses
    ("( 3 - 8 * (3 + 1)))", "", False),
    ("(37 * 9)) + 3 - 245", "", False),
    ("56 + (37 * 9)) + 3 - 269", "", False),
    ("((24 * (37 / 9)) / 86 - 962", "", False),

    # Division by "zero"
    ("128 - (37 + 90) / (15 - 5 * 3)", "", False),
    ("275 + 1 / (78 / (1000 * 7 * (91 + 111)) )- 28", "", True)
]

def program(arithmetic_expression, float_mode=False):
    command = ["build/app.exe"]
    if float_mode:
        command.append("--float")
    
    result = subprocess.Popen(
        command,
        stdin  = subprocess.PIPE,
        stdout = subprocess.PIPE,
        stderr = subprocess.PIPE,
        text = True
    )
    output = result.communicate(input=arithmetic_expression + "\n")[0]
    return result.returncode, output.strip()

@pytest.mark.parametrize("arithmetic_expression, expected_output, type_numbers", correct_arithmetic_equations)
def test_correct_input(arithmetic_expression, expected_output, type_numbers):
    return_code, output = program(arithmetic_expression, type_numbers)
    assert return_code == 0
    assert output == expected_output

@pytest.mark.parametrize("arithmetic_expression, expected_output, type_numbers", erroneous_arithmetic_equations)
def test_erroneous_input(arithmetic_expression, expected_output, type_numbers):
    return_code, output = program(arithmetic_expression, type_numbers)
    assert return_code != 0
    assert output == expected_output
