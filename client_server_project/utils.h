class Function {
public:
	int id;
        int arg_1;
        int arg_2;
        int result;
};

int addition(int number1, int number2)
{
	return number1 + number2;
}

int substraction(int number1, int number2)
{
	return number1 - number2;
}

int multiplication(int number1, int number2)
{
	return number1 * number2;
}

int division(int number1, int number2)
{
	if (number2 == 0)
		return INT_MAX;

	return number1 / number2;
}
