import QtQuick 2.0

import "computacaoBtnFunctions.js" as ComputacaoBtnFunction

CommonBtn {

    id:btn

    property bool geoToggle: false

    mouseArea.onClicked: {
        switch(texto) {
        case "BIN":

            textDisplay.text = ComputacaoBtnFunction.toBin(textDisplay.text)

            break

        case "OCT":

            textDisplay.text = ComputacaoBtnFunction.toOct(textDisplay.text)

            break

        case "HEX":

            textDisplay.text = ComputacaoBtnFunction.toHex(textDisplay.text)

            break


        default:
            if(texto === "C"){
                ComputacaoBtnFunction.backspace(textDisplay.text)
            } else {
                if(!tipo)
                    textDisplay.text += texto
                else {
                    textDisplay.text += texto
                    operators.enabled = false;
                }
            }
        }
    }
    mouseArea.onPressAndHold: {

        if(texto === "C"){
            btn.color=Qt.lighter(btn.color, 1.3);
            textDisplay.text = ""
        }
    }
}


