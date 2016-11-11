package com.vintage.vintagechess;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.PorterDuff;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Space;
import android.widget.Toast;

import org.json.JSONException;

import java.util.ArrayList;

public class ActivityGame extends AppCompatActivity {

    ImageView board;
    ImageView motionlessPieces;
    ImageView movingPiece;
    Space leftSpace;
    private static boolean gameStarted = false;

    RadioGroup selectStyle;

    @Override
    protected  void onResume() {
        super.onResume();
        Utilities.currentActivity = this;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        gameStarted = false;

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);

        board = (ImageView) findViewById(R.id.imageViewBoard);
        board.setImageResource(android.R.color.transparent);

        motionlessPieces = (ImageView) findViewById(R.id.imageViewMotionlessPieces);
        motionlessPieces.setImageResource(android.R.color.transparent);

        movingPiece = (ImageView) findViewById(R.id.imageViewMovingPieces);
        movingPiece.setImageResource(android.R.color.transparent);

        leftSpace = (Space) findViewById(R.id.leftSpace);

        board.setOnTouchListener(new View.OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
                handleTouch(event);
                return true;
            }
        });

        selectStyle = (RadioGroup) findViewById(R.id.RadioGroupSelect);
        selectStyle.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                handleRadioChange();
            }
        });


        Display.board = board;
        Display.motionlessPieces = motionlessPieces;
        Display.movingPiece = movingPiece;


    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        board.requestLayout();
        Game.initializeGame();
        Display.drawFullBoard();
        Game.leftSpace = leftSpace.getWidth();

        handleRadioChange();
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();

        try {
            HttpRunner.runPostGameEnd();
            // reste?

        } catch (JSONException e) {
            e.printStackTrace();
        }

        Intent setIntent = new Intent(this,ActivityCreateGame.class);
        startActivity(setIntent);

    }

    public String positionToSymbol(int x, int y)
    {
        int valXX = ((x-130)/151);
        String valX = Character.toString ((char) (valXX+97));
        //char valX =  (char) valXX;  /// affiche un carre faut arranger ca
        int valY = 8 - ((y-62)/151);

        String symbole = valX+String.valueOf(valY);
        //Log.d("symbole ", symbole );

        return symbole;

    }

    private void handleTouch(MotionEvent event) {
        try {

            int action = event.getAction();
            if (!gameStarted) {
                HttpRunner.runPostGameStart();
                gameStarted = true;
            }
            switch (action) {
                case MotionEvent.ACTION_DOWN:
                    Game.handleFingerDown((int)event.getX(), (int)event.getY());
                    break;
                case MotionEvent.ACTION_MOVE:
                    Game.handleMove((int)event.getX(), (int)event.getY());
                    break;
                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL :
                    Game.handleFingerUp();
                    break;
            }

        }
        catch (Exception e) {
            e.printStackTrace();
        }
        // TEST DELETE PIECE
        //Game.deletePiece("a1");
        //Game.deletePiece("e5");
    }

    private void handleRadioChange() {
        applyFilter();
        changeStyle();
        Display.setBoardImg();
        Display.drawFullBoard();
    }

    private void applyFilter() {
        for (int i = 0 ; i < selectStyle.getChildCount() ; i++) {
            if ( i % 2 == 0) {
                View tmpRadio = selectStyle.getChildAt(i);
                //Log.d("", selectStyle.getChildCount()+"");
                tmpRadio.getBackground().clearColorFilter();
            }
        }
        int radioButtonID = selectStyle.getCheckedRadioButtonId();
        View radioButton = selectStyle.findViewById(radioButtonID);
        radioButton.getBackground().setColorFilter(Color.GRAY, PorterDuff.Mode.MULTIPLY);
    }

    private void changeStyle() {
        int radioButtonID = selectStyle.getCheckedRadioButtonId();
        View radioButton = selectStyle.findViewById(radioButtonID);
        int index = selectStyle.indexOfChild(radioButton)/2;
        if (index % 2 == 0) {
            Game.style = "1";
        }
        else {
            Game.style = "2";
        }
        if (index / 2 == 0) {
            Display.board_style = "red";
        }
        else if (index / 2 == 1) {
            Display.board_style = "blue";
        }
        else {
            Display.board_style = "wood";
        }
    }

}









