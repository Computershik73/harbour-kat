import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.WebView 1.0
import Sailfish.WebEngine 1.0

Dialog {
    id: authTwoFactor
    allowedOrientations: Orientation.Portrait
    property string redirect_uri
    property bool goback: webView1.url != redirect_uri

    WebView {
        id: webView1
         anchors.fill: authTwoFactor
         url: redirect_uri
         viewportHeight: parent.height
    }

    onGobackChanged: {
        if(goback) {
            authTwoFactor.accept();
        }
    }

    onAccepted: {
        vksdk.auth.sendAnswer(webView1.url.toString())
    }
}
