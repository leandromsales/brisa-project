import QtQuick 2.0

import "geometryBtnFunctions.js" as GeometryBtnFunction

CommonBtn {

    id:btn

    property bool geoToggle: false

    mouseArea.onClicked: {
        switch(texto) {
        case "Sen":

            textDisplay.text = Math.sin(GeometryBtnFunction.toRadians(textDisplay.text))

            break

        case "Cos":

            textDisplay.text = Math.cos(GeometryBtnFunction.toRadians(textDisplay.text))

            break

        case "Tan":

            textDisplay.text = Math.tan(GeometryBtnFunction.toRadians(textDisplay.text))

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

        btn.color=Qt.lighter(btn.color, 1.3);

        if(texto === "C"){
            textDisplay.text = ""
        }
    }
}


