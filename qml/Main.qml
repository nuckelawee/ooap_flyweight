import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ParticleSystem 1.0
import ParticleRenderer 1.0

Window {
    id: root
    width: 1200
    height: 800
    visible: true
    title: "Particle Simulation - WITH Flyweight Pattern"

    ParticleSystem {
        id: particleSystem
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Main rendering area - C++ QPainter renderer (FAST!)
        ParticleRenderer {
            id: renderer
            Layout.fillWidth: true
            Layout.fillHeight: true
            particleSystem: particleSystem

            onWidthChanged: {
                if (width > 0 && height > 0) {
                    particleSystem.start(width, height);
                }
            }

            onHeightChanged: {
                if (width > 0 && height > 0) {
                    particleSystem.start(width, height);
                }
            }

            // Обновляем рендер каждый кадр
            Timer {
                id: renderTimer
                interval: 16  // ~60 FPS
                running: false
                repeat: true
                onTriggered: renderer.update()
            }

            Component.onCompleted: {
                renderTimer.start();
            }
        }

        // Control panel
        Rectangle {
            Layout.preferredWidth: 320
            Layout.fillHeight: true
            color: "#ffffff"
            border.color: "#e0e0e0"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                Text {
                    text: "WITH FLYWEIGHT"
                    font.pixelSize: 18
                    font.bold: true
                    color: "#27ae60"
                    Layout.alignment: Qt.AlignHCenter
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 2
                    color: "#e0e0e0"
                }

                // Metrics display
                GroupBox {
                    title: "Metrics"
                    Layout.fillWidth: true

                    background: Rectangle {
                        color: "#fafafa"
                        border.color: "#d0d0d0"
                        radius: 5
                    }

                    label: Label {
                        color: "#333333"
                        text: parent.title
                        font.bold: true
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10

                        MetricRow {
                            label: "FPS:"
                            value: particleSystem.fps.toFixed(1)
                            valueColor: particleSystem.fps > 30 ? "#4ecdc4" : "#ff6b6b"
                        }

                        MetricRow {
                            label: "Particles:"
                            value: particleSystem.particleCount.toString()
                            valueColor: "#95e1d3"
                        }

                        MetricRow {
                            label: "Flyweights:"
                            value: particleSystem.objectCount.toString()
                            valueColor: "#ffd93d"
                        }
                    }
                }

                // Controls
                GroupBox {
                    title: "Controls"
                    Layout.fillWidth: true

                    background: Rectangle {
                        color: "#fafafa"
                        border.color: "#d0d0d0"
                        radius: 5
                    }

                    label: Label {
                        color: "#333333"
                        text: parent.title
                        font.bold: true
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10

                        RowLayout {
                            Layout.fillWidth: true

                            TextField {
                                id: particleCountInput
                                Layout.fillWidth: true
                                text: "1000"
                                placeholderText: "Count"
                                color: "#333333"

                                background: Rectangle {
                                    color: "#ffffff"
                                    border.color: "#cccccc"
                                    radius: 3
                                }
                            }
                        }

                        Button {
                            text: "Add Particles"
                            Layout.fillWidth: true
                            onClicked: {
                                var count = parseInt(particleCountInput.text);
                                if (count > 0) {
                                    particleSystem.addParticles(count);
                                    if (!renderTimer.running) {
                                        renderTimer.start();
                                    }
                                }
                            }

                            background: Rectangle {
                                color: parent.pressed ? "#3a7bc8" : "#4a90e2"
                                radius: 5
                            }

                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Button {
                            text: "Throw Up Particles"
                            Layout.fillWidth: true
                            onClicked: {
                                particleSystem.throwUpParticles();
                            }

                            background: Rectangle {
                                color: parent.pressed ? "#27ae60" : "#2ecc71"
                                radius: 5
                            }

                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Button {
                            text: "Clear All"
                            Layout.fillWidth: true
                            onClicked: {
                                particleSystem.clear();
                                renderTimer.stop();
                                renderer.update();
                            }

                            background: Rectangle {
                                color: parent.pressed ? "#c44545" : "#e74c3c"
                                radius: 5
                            }

                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }

                // Quick add buttons
                GroupBox {
                    title: "Quick Add"
                    Layout.fillWidth: true

                    background: Rectangle {
                        color: "#fafafa"
                        border.color: "#d0d0d0"
                        radius: 5
                    }

                    label: Label {
                        color: "#333333"
                        text: parent.title
                        font.bold: true
                    }

                    GridLayout {
                        anchors.fill: parent
                        columns: 2
                        rowSpacing: 8
                        columnSpacing: 8

                        QuickAddButton {
                            text: "+ 100"
                            onClicked: {
                                particleSystem.addParticles(100);
                                if (!renderTimer.running) renderTimer.start();
                            }
                        }

                        QuickAddButton {
                            text: "+ 500"
                            onClicked: {
                                particleSystem.addParticles(500);
                                if (!renderTimer.running) renderTimer.start();
                            }
                        }

                        QuickAddButton {
                            text: "+ 1000"
                            onClicked: {
                                particleSystem.addParticles(1000);
                                if (!renderTimer.running) renderTimer.start();
                            }
                        }

                        QuickAddButton {
                            text: "+ 5000"
                            onClicked: {
                                particleSystem.addParticles(5000);
                                if (!renderTimer.running) renderTimer.start();
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }

                Text {
                    text: "общие данные (цвет, размер, форма)\nразделяются между частицами\n(Flyweight Pattern)"
                    font.pixelSize: 11
                    color: "#666666"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                }
            }
        }
    }

    // Custom components
    component MetricRow: RowLayout {
        property string label: ""
        property string value: ""
        property color valueColor: "#ffffff"

        Layout.fillWidth: true
        spacing: 10

        Text {
            text: parent.label
            font.pixelSize: 14
            color: "#666666"
            Layout.preferredWidth: 80
        }

        Text {
            text: parent.value
            font.pixelSize: 16
            font.bold: true
            color: parent.valueColor
            Layout.fillWidth: true
        }
    }

    component QuickAddButton: Button {
        Layout.fillWidth: true

        background: Rectangle {
            color: parent.pressed ? "#b0b0b0" : "#e0e0e0"
            radius: 3
            border.color: "#c0c0c0"
            border.width: 1
        }

        contentItem: Text {
            text: parent.text
            color: "#333333"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 12
        }
    }
}
