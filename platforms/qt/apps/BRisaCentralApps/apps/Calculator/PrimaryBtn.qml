import QtQuick 2.0

import "./primaryBtnFunctions.js" as PrimaryBtnFunction

CommonBtn {

    id:btn

    property bool geoToggle: false

    mouseArea.onClicked: {
        switch(texto) {
        case "=":

            var equation = textDisplay.text

            operators.enabled = true;
            textDisplay.text = ""

            PrimaryBtnFunction.operation(equation)

            break

        case "R" :

            textDisplay.text = Math.sqrt(textDisplay.text)

            break

        case "GEO" :

            if(!geoToggle) {
                geoToggle = true
                btn.color=Qt.lighter(btn.color, 1.4)
            } else {
                geoToggle = false
                btn.color="lightblue"
            }

            geometryPanel.visible = geoToggle

            break

        default:
            if(texto === "C"){
                PrimaryBtnFunction.backspace(textDisplay.text)
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


