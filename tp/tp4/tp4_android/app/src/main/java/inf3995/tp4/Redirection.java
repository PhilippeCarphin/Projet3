package inf3995.tp4;

import android.content.Intent;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;

//
//La classe pour la nouvelle page affichée avec le load de l url
//
//
public class Redirection extends AppCompatActivity {

    public Button returnButton;
    public WebView webView;
    public static String url;
    public static int testNum;

    //
    //Fonction qui crée la page avec les settings et l affichage de la vue
    //
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        //setter la vue
        setContentView(R.layout.activity_redirection);
        webView = (WebView) findViewById(R.id.webView);
        //settings
        WebSettings settings = webView.getSettings();
        settings.setDefaultTextEncodingName("utf-8 ");
        webView.getSettings().setBuiltInZoomControls(true);
        webView.getSettings().setUseWideViewPort(true);
        webView.getSettings().setJavaScriptEnabled(true);
        webView.getSettings().setLoadWithOverviewMode(true);
//        if (testNum < 2 ) {
//            webView.getSettings().setTextZoom(512);
//        }

        //loard l url
        webView.loadUrl(url);

        //setter la vue
        webView.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return false;
            }
        });

        // Le bouton de retour
        returnButton = (Button) findViewById(R.id.returnButton);
        returnButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                //Procedure pour le bouton
                Intent intent = new Intent(v.getContext(), TP4.class);
                startActivity(intent);
            }
        });
    }



}
