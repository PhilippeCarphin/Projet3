package com.vintage.vintagechess;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.PorterDuff;
import android.graphics.drawable.shapes.RoundRectShape;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Space;
import android.widget.TextClock;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONException;

import java.util.ArrayList;

public class ActivityGame extends AppCompatActivity {

    ImageView board;
    ImageView motionlessPieces;
    ImageView movingPiece;
    Space leftSpace;
    TextView player1Name;
    TextView player2Name;
    TextClock timerPlaye1;
    TextClock timerPlaye2;
    TextView round;
    static TextView turn;
    static TextView moveNumber;
    TextView location;
    private Button buttonEnd;

    private static boolean gameStarted = false;

    RadioGroup selectStyle;

    @Override
    protected  void onResume() {
        super.onResume();
        Utilities.currentActivity = this;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        try {
            Utilities.currentActivity = this;
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

            player1Name = (TextView) findViewById(R.id.Player1Name);
            player2Name = (TextView) findViewById(R.id.Player2Name);
            location = (TextView) findViewById(R.id.LocationText);

            timerPlaye1 = (TextClock) findViewById(R.id.TimePlayer1);
            timerPlaye2 = (TextClock) findViewById(R.id.TimePlayer2);
            round = (TextView) findViewById(R.id.RoundText);
            turn = (TextView) findViewById(R.id.CurrentPlayer);
            moveNumber = (TextView) findViewById(R.id.MoveNumber);
            player1Name.setText(ActivityCreateGame.playerName1);
            player2Name.setText(ActivityCreateGame.playerName2);
            location.setText(ActivityCreateGame.location);
            round.setText("Round : 0" );
        }
        catch (Exception e) {
            Utilities.messageBox("Failed to create game", e.getMessage());
        }


    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        try {

        super.onWindowFocusChanged(hasFocus);
        board.requestLayout();
        if (!gameStarted) {
            //Game.initializeGame();
            Utilities.messageBoxStartGame();
            gameStarted = true;
        }


        handleRadioChange();
        }
        catch (Exception e) {
            Utilities.messageBox("Failed to change focus", e.getMessage());
        }
    }

    @Override
    public void onBackPressed() {
        //super.onBackPressed();

        try {
            HttpRunner.runPostGameEnd();
        } catch (JSONException e) {
            Utilities.messageBox("Error after having pressed back", e.getMessage());
        }


    }



    @Override
    public void onDestroy()
    {
        try {
            HttpRunner.runPostGameEnd();

        } catch (JSONException e) {
            e.printStackTrace();
            Utilities.messageBox("Error after destroying view", e.getMessage());
        }
        super.onDestroy();
    }

    private void handleTouch(MotionEvent event) {
        try {

            int action = event.getAction();
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
            //e.printStackTrace();
            Game.recoverFromError();
            if (!e.getMessage().contains("Finger is out of bounds"))
                Utilities.messageBox("Error with view touch event", e.getMessage());
        }

    }

    private void handleRadioChange() {
        try {
            applyFilter();
            changeStyle();
            //Display.setBoardImg();
            Display.drawFullBoard();
        }
        catch (Exception e) {
            //e.printStackTrace();
            Utilities.messageBox("Error with radio change", e.getMessage());
        }
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









