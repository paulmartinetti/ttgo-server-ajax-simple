const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style type="text/css">
        body {
            background-color: white;
            color: black;
            width: 40pc;
            padding: 5px;
        }

        .wrapper {
            width: 400px;
            height: auto;
            margin: 0 auto;
            position: relative;
            border: 1px solid #999;
            padding: 5px;
        }

        .console {
            width: 380px;
            height: auto;
            margin: 0 auto;
            position: relative;
            display: flex;
            border: 1px solid #999;
            padding: 5px;
        }

        .screen {
            width: 20pc;
            height: auto;
            margin: 0 auto;
            position: relative;
            padding: 5px;
        }

        .commPort {
            width: 60pc;
            height: auto;
            margin: 0 auto;
            position: relative;
            display: inline-block;
            padding: 5px;
        }

        .textBox {
            font-family: Verdana, Geneva, Tahoma, sans-serif;
            padding: 5px;
        }

        .textInput {
            width: 200px;
            padding: 5px;
        }

    </style>
</head>
<body>
    <div class="wrapper">
        <div class="console">
            <div class="screen">
                <div class="textBox" id="onOrOff"></div>
                <button onclick="setScreen()">Screen</button>
            </div>
            <div class="commPort">
                <input class="textInput" id="textBox" type="text"/>
            </div>
        </div>
        <script>
            let status;
            let timer;
            function getStatus() {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                        // server returns Off or On
                        document.getElementById("onOrOff").innerHTML = this.responseText;
                    }
                };
                xhttp.open("GET", "get_status", true);
                xhttp.send();
            }
            function setScreen() {
                // get status
                let s = document.getElementById("onOrOff").innerHTML;
                // toggle
                status = (s == "Off") ? 1 : 0;
                // 
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                        // server returns Off or On
                        let r = this.responseText;
                        document.getElementById("onOrOff").innerHTML = r;
                        // stop timer if screen off
                        (r == "On") ? startTimer() : stopTimer();
                    }
                };
                xhttp.open("GET", "set_screen?screen=" + status, true);
                xhttp.send();
            }
            function setText () {
                let t = document.getElementById("textBox").value;
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.status == 200) {
                        console.log("ok");
                    } else {
                        console.log("error");
                    }
                };
                xhttp.open("GET", "set_text?text=" + t, true);
                xhttp.send();
            }
            function startTimer() {
                timer = setInterval(function () {
                    setText();
                }, 2000);
            }
            function stopTimer() {
                clearInterval(timer);
            }
            // c'est parti
            getStatus();
        </script>
    </div>
</body>
</html>
)=====";
