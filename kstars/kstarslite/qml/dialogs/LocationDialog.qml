import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import "../constants" 1.0
import "../modules"

KSPage {
    contentItem: locationColumn
    title: xi18n("Set Geolocation")

    function filterCities() {
        LocationDialogLite.filterCity(cityFilter.text, provinceFilter.text, countryFilter.text)
    }

    onVisibleChanged: {
        filterCities()
    }

    ColumnLayout {
        id: locationColumn
        spacing: 5 * num.dp
        anchors{
            bottom: parent.bottom
            bottomMargin: 15 * num.dp
        }

        Flow {
            anchors {
                left: parent.left
                right: parent.right
            }

            Label {
                text: xi18n("Current Location: ")
            }

            Label {
                text: LocationDialogLite.currentLocation
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "grey"
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            flow: window.isPortrait ? GridLayout.TopToBottom : GridLayout.LeftToRight

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Label {
                    text: xi18n("City filter: ")
                }
                TextField {
                    id: cityFilter
                    Layout.fillWidth: true
                    onTextChanged: {
                        filterCities()
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Label {
                    text: xi18n("Province filter: ")
                }

                TextField {
                    id: provinceFilter
                    Layout.fillWidth: true
                    onTextChanged: {
                        filterCities()
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Label {
                    text: xi18n("Country filter: ")
                }
                TextField {
                    id: countryFilter
                    Layout.fillWidth: true
                    onTextChanged: {
                        filterCities()
                    }
                }
            }
        }

        KSListView {
            model: CitiesModel
            textRole: "display"

            Layout.fillWidth: true
            Layout.fillHeight: true

            checkCurrent: true
            currentIndex: LocationDialogLite.currLocIndex
            onClickCheck: false

            onClicked: {
                locationsGeoMenu.openMenu(text)
            }
        }

        Button {
            anchors {
                bottom: parent.bottom
            }

            text: "Add Location"
            onClicked: {
                locationEdit.openAdd()
            }
        }
    }
}
