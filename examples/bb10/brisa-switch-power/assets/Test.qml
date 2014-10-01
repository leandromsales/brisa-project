import bb.cascades 1.2

Page {
    Container {
        Label {
            id: zooey
            text: "Zodiac picker"
        } // end of Label
        Picker {
            id: zodiac
            property string myValue: "Aries - Rat"
            title: "Zodiac"
            description: zodiac.myValue
            dataModel: XmlDataModel {
                source: "asset:///zodiacvalues.xml"
            } // end of dataModel
            pickerItemComponents: [
                PickerItemComponent {
                    type: "itemmonth"
                    content: Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        } // end of StackLayout
                        ImageView {
                            imageSource: pickerItemData.filename
                        } // end of ImageView
                        Label {
                            text: pickerItemData.displayname
                        } // end of Label
                    } // end of Container
                },// end of PickerItemComponent
                PickerItemComponent {
                    type: "itemyear"
                    content: Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        } // end of StackLayout
                        ImageView {
                            imageSource: pickerItemData.filename
                        } // end of ImageView
                        Label {
                            text: pickerItemData.displayname
                        } // end of Label
                    } // end of Container
                } // end of PickerItemComponent
            ]// end of pickerItemComponents
            onSelectedValueChanged: {
                var monthName
                switch (zodiac.selectedIndex(0)) {
                    case 0:
                        monthName = "Aries";
                        break;
                    case 1:
                        monthName = "Taurus";
                        break;
                    case 2:
                        monthName = "Gemini";
                        break;
                    case 3:
                        monthName = "Cancer";
                        break;
                    case 4:
                        monthName = "Leo";
                        break;
                    case 5:
                        monthName = "Virgo";
                        break;
                    case 6:
                        monthName = "Libra";
                        break;
                    case 7:
                        monthName = "Scorpio";
                        break;
                    case 8:
                        monthName = "Sagittarius";
                        break;
                    case 9:
                        monthName = "Capricorn";
                        break;
                    case 10:
                        monthName = "Aquarius";
                        break;
                    case 11:
                        monthName = "Pisces";
                        break;
                }// end of switch
                var yearName
                switch (zodiac.selectedIndex(1)) {
                    case 0:
                        yearName = "Rat";
                        break;
                    case 1:
                        yearName = "Ox";
                        break;
                    case 2:
                        yearName = "Tiger";
                        break;
                    case 3:
                        yearName = "Rabbit";
                        break;
                    case 4:
                        yearName = "Dragon";
                        break;
                    case 5:
                        yearName = "Snake";
                        break;
                    case 6:
                        yearName = "Horse";
                        break;
                    case 7:
                        yearName = "Goat";
                        break;
                    case 8:
                        yearName = "Monkey";
                        break;
                    case 9:
                        yearName = "Rooster";
                        break;
                    case 10:
                        yearName = "Dog";
                        break;
                    case 11:
                        yearName = "Pig";
                        break;
                }// end of switch
                zodiac.myValue = monthName + " - " + yearName
            }// end of onSelectedValueChanged
        }// end of Picker
        Label {
            id: zodiacLabel
            text: zodiac.myValue
        }// end of Label
    }// end of Container
}// end of Page