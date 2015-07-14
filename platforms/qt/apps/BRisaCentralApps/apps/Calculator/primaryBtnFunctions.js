function backspace(equation) {
    textDisplay.text = equation.substring(0, equation.length - 1);
}
function operation(equation) {

    var numbers;
    var resultado;

    if(equation.indexOf("+") > -1) {

        numbers = equation.split("+")

        textDisplay.text = parseFloat(parseFloat(numbers[0])
                                      +
                                      parseFloat(numbers[1]))


    }
    else if(equation.indexOf("-") > -1) {
        numbers = equation.split("-")
        textDisplay.text = parseFloat(parseFloat(numbers[0])
                                      -
                                      parseFloat(numbers[1]));
    }
    else if(equation.indexOf("*") > -1) {
        numbers = equation.split("*")
        textDisplay.text = parseFloat(parseFloat(numbers[0])
                                      *
                                      parseFloat(numbers[1]));
    }
    else if(equation.indexOf("/") > -1) {
        numbers = equation.split("/")
        textDisplay.text = parseFloat(parseFloat(numbers[0])
                                      /
                                      parseFloat(numbers[1]));
    }
    else if(equation.indexOf("^") > -1) {
        numbers = equation.split("^")
        textDisplay.text = parseFloat(Math.pow(parseFloat(numbers[0]),
                                      parseFloat(numbers[1])));
    }
}
