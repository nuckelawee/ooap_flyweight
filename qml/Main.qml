import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ParticleSystem 1.0

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

        // Main canvas area
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f5f5f5"

            // Pre-rendered offscreen canvases для ParticleTypes (Flyweight rendering!)
            Item {
                id: particleTypeCanvases
                visible: false

                // Каждый Canvas pre-renders один ParticleType
                Canvas {
                    id: redCircle
                    width: 24; height: 24
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.fillStyle = Qt.rgba(220/255, 50/255, 50/255, 1);
                        ctx.strokeStyle = Qt.darker(Qt.rgba(220/255, 50/255, 50/255, 1), 1.5);
                        ctx.lineWidth = 2;
                        ctx.beginPath();
                        ctx.arc(12, 12, 8, 0, Math.PI * 2);
                        ctx.fill();
                        ctx.stroke();
                    }
                    Component.onCompleted: requestPaint()
                }

                Canvas {
                    id: greenSquare
                    width: 20; height: 20
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.fillStyle = Qt.rgba(50/255, 180/255, 50/255, 1);
                        ctx.strokeStyle = Qt.darker(Qt.rgba(50/255, 180/255, 50/255, 1), 1.5);
                        ctx.lineWidth = 2;
                        ctx.beginPath();
                        ctx.rect(4, 4, 12, 12);
                        ctx.fill();
                        ctx.stroke();
                    }
                    Component.onCompleted: requestPaint()
                }

                Canvas {
                    id: blueTriangle
                    width: 28; height: 28
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.fillStyle = Qt.rgba(50/255, 50/255, 220/255, 1);
                        ctx.strokeStyle = Qt.darker(Qt.rgba(50/255, 50/255, 220/255, 1), 1.5);
                        ctx.lineWidth = 2;
                        ctx.beginPath();
                        ctx.moveTo(14, 4);
                        ctx.lineTo(4, 24);
                        ctx.lineTo(24, 24);
                        ctx.closePath();
                        ctx.fill();
                        ctx.stroke();
                    }
                    Component.onCompleted: requestPaint()
                }

                Canvas {
                    id: yellowCircle
                    width: 22; height: 22
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.fillStyle = Qt.rgba(200/255, 180/255, 0, 1);
                        ctx.strokeStyle = Qt.darker(Qt.rgba(200/255, 180/255, 0, 1), 1.5);
                        ctx.lineWidth = 2;
                        ctx.beginPath();
                        ctx.arc(11, 11, 7, 0, Math.PI * 2);
                        ctx.fill();
                        ctx.stroke();
                    }
                    Component.onCompleted: requestPaint()
                }

                Canvas {
                    id: magentaSquare
                    width: 26; height: 26
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.fillStyle = Qt.rgba(180/255, 50/255, 180/255, 1);
                        ctx.strokeStyle = Qt.darker(Qt.rgba(180/255, 50/255, 180/255, 1), 1.5);
                        ctx.lineWidth = 2;
                        ctx.beginPath();
                        ctx.rect(4, 4, 18, 18);
                        ctx.fill();
                        ctx.stroke();
                    }
                    Component.onCompleted: requestPaint()
                }
            }

            Canvas {
                id: canvas
                anchors.fill: parent

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

                // Получить pre-rendered canvas для ParticleType
                function getParticleTypeCanvas(color, radius, shape) {
                    // Сопоставляем с pre-rendered canvases
                    var r = color.r * 255;
                    var g = color.g * 255;
                    var b = color.b * 255;

                    // Red circle
                    if (Math.abs(r - 220) < 5 && Math.abs(g - 50) < 5 && Math.abs(b - 50) < 5 && shape === 0) {
                        return {canvas: redCircle, offsetX: 12, offsetY: 12};
                    }
                    // Green square
                    if (Math.abs(r - 50) < 5 && Math.abs(g - 180) < 5 && Math.abs(b - 50) < 5 && shape === 1) {
                        return {canvas: greenSquare, offsetX: 10, offsetY: 10};
                    }
                    // Blue triangle
                    if (Math.abs(r - 50) < 5 && Math.abs(g - 50) < 5 && Math.abs(b - 220) < 5 && shape === 2) {
                        return {canvas: blueTriangle, offsetX: 14, offsetY: 14};
                    }
                    // Yellow circle
                    if (Math.abs(r - 200) < 5 && Math.abs(g - 180) < 5 && b < 5 && shape === 0) {
                        return {canvas: yellowCircle, offsetX: 11, offsetY: 11};
                    }
                    // Magenta square
                    if (Math.abs(r - 180) < 5 && Math.abs(g - 50) < 5 && Math.abs(b - 180) < 5 && shape === 1) {
                        return {canvas: magentaSquare, offsetX: 13, offsetY: 13};
                    }

                    return null;
                }

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.clearRect(0, 0, width, height);

                    var particles = particleSystem.getParticleData();

                    // FLYWEIGHT RENDERING: используем pre-rendered изображения!
                    for (var i = 0; i < particles.length; i++) {
                        var p = particles[i];
                        var typeCanvas = getParticleTypeCanvas(p.color, p.radius, p.shape);

                        if (typeCanvas && typeCanvas.canvas.available) {
                            // Быстрая отрисовка: копируем готовое изображение!
                            ctx.drawImage(typeCanvas.canvas,
                                         p.x - typeCanvas.offsetX,
                                         p.y - typeCanvas.offsetY);
                        }
                    }
                }

                Timer {
                    id: renderTimer
                    interval: 16
                    running: false
                    repeat: true
                    onTriggered: canvas.requestPaint()
                }
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

                        MetricRow {
                            label: "Memory:"
                            value: particleSystem.memoryUsageMB.toFixed(2) + " MB"
                            valueColor: "#f38181"
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
                                canvas.requestPaint();
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
