package inf3995.tp4;

import android.content.Intent;
import android.net.Uri;
import android.support.v4.view.MotionEventCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.content.Context;
//
// La classe pour la page principale affichée
//
//
public class TP4 extends AppCompatActivity {

    //initialisation + variables
    private static int testNum = 0;
    public Button button1;
    public float y1,y2;

    //
    // onCreate: fonction pour créer la vue et rediriger si le bouton est cliqué
    //
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tp4);
        button1 = (Button) findViewById(R.id.button1);
        button1.setText("test" + Integer.valueOf(testNum + 1).toString());
        //Quand on clique sur le bouton
        button1.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Redirection.testNum = testNum;
                Redirection.url="http://132.207.89.24/test" + Integer.valueOf(testNum+1).toString();
                Intent intent = new Intent(v.getContext(), Redirection.class);
                startActivity(intent);
            }
        });
    }

    //
    //  onTouchEvent : fonction declanchée lorsqu'on touche l écran
    //
    @Override
    public boolean onTouchEvent(MotionEvent event){

        int action = MotionEventCompat.getActionMasked(event);
        if (action == MotionEvent.ACTION_UP) {
        //changer de bouton
            y2 = event.getY();
            if (y2 < y1) {
                testNum = (testNum + 1) % 3;
                button1.setText("test" + Integer.valueOf(testNum + 1).toString());
            }
        }
        else if (action == MotionEvent.ACTION_DOWN) {
            y1 = event.getY();
        }
        return true;
    }

}
