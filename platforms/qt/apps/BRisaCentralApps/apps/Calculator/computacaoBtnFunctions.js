function backspace(equation) {
    textDisplay.text = equation.substring(0, equation.length - 1);
}
function toBin(num) {
    return parseFloat(parseFloat(num)).toString(2);
}
function toOct(num) {
    return parseFloat(parseFloat(num)).toString(8);
}
function toHex(num) {
    return parseFloat(parseFloat(num)).toString(16);
}
