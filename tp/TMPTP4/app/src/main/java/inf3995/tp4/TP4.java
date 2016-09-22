package inf3995.tp4;

import android.content.Intent;
import android.support.v4.view.MotionEventCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.content.Context;

public class TP4 extends AppCompatActivity {


    public int testNum;
    public Button button1;
    public float y1,y2;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tp4);
        testNum = 0;
        button1 = (Button) findViewById(R.id.button1);
        button1.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                //Procedure pour le bouton
                Intent intent = new Intent(v.getContext(), Redirection.class);
                startActivity(intent);
            }
        });
    }

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
        System.out.println(testNum);

        return true;
    }

}
