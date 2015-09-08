import QtQuick 2.4
import QtQuick.Controls 1.3

import QtMultimedia 5.4

Rectangle {

    anchors.fill: parent

    Camera {

        id: camera

        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

        exposure {
            exposureCompensation: -1.0
            exposureMode: Camera.ExposurePortrait
        }

        imageProcessing {
            whiteBalanceMode: Camera.WhiteBalanceTungsten
            contrast: 0.66
            saturation: -0.5
        }

        flash.mode: Camera.FlashRedEyeReduction

        imageCapture {
            onImageCaptured: {
                photoPreview.visible = true;
                stackPages.push(photoPreview);
                photoPreview.source = preview  // Show the preview in an Image
            }
        }
    }

    StackView {
        id:stackPages
        anchors.fill: parent

        initialItem: videoOutput

        Rectangle {
            id:videoOutput

            VideoOutput {
                source: camera

                height:parent.height*(0.9)
                width:parent.width

                focus : visible // to receive focus and capture key events when visible
            }

            Rectangle {
                width:parent.width
                height:parent.height*(0.1)

                anchors.bottom: parent.bottom

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter

                    width:parent.width/5
                    height:parent.height

                    radius:10
                    color:"#343434"

                    Text {
                        anchors.fill: parent

                        text:"Shot!"
                        color:"white"
                        font.bold: true
                        font.pixelSize: parent.height

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: camera.imageCapture.capture()
                    }
                }
            }
        }
    }

    Image {
        id:photoPreview
        visible:false

        MouseArea {
            anchors.fill:parent
            property real initialX

            onPressed: {
                initialX = mouseX;
            }

            onReleased: {
                if(mouseX > initialX) {
                    stackPages.pop();
                }
            }
        }
    }

}

