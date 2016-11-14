package com.vintage.vintagechess;
import android.content.Intent;

import android.graphics.PorterDuff;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Space;
import android.widget.TextClock;
import android.widget.TextView;

import org.json.JSONException;

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
    int radioButtonID;
    View radioButton;
    static int index;
    static View radioButtonStyle;
    View radioButtonColor;
    RadioGroup RadioGroupSelectColorBoard;
    RadioGroup RadioGroupSelectStylePiece;
    public static RadioButton radioButtonStyle1, radioButtonStyle2, radioButtonColorRed, radioButtonColorblue, radioButtonColorWood;

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

            radioButtonStyle1 = (RadioButton) findViewById(R.id.radioButtonStyle1);
            radioButtonStyle2 = (RadioButton) findViewById(R.id.radioButtonStyle2);

            radioButtonColorRed = (RadioButton) findViewById(R.id.radioButtonRed);
            radioButtonColorblue = (RadioButton) findViewById(R.id.radioButtonBlue);
            radioButtonColorWood = (RadioButton) findViewById(R.id.radioButtonWood);

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

            /*selectStyle = (RadioGroup) findViewById(R.id.RadioGroupSelect);
            selectStyle.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(RadioGroup group, int checkedId) {
                    handleRadioChange();
                }
            });
            radioButtonID = selectStyle.getCheckedRadioButtonId();
            radioButton = selectStyle.findViewById(radioButtonID);*/


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
             //
            // index = selectStyle.indexOfChild(radioButton)/2;

            RadioGroupSelectColorBoard = (RadioGroup) findViewById(R.id.RadioGroupBoardColor);
            RadioGroupSelectColorBoard.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(RadioGroup group, int checkedId) {
                    handleRadioButtonBoardColorChange(RadioGroupSelectColorBoard, radioButtonColor);
                }
            });

            RadioGroupSelectStylePiece = (RadioGroup) findViewById(R.id.RadioGroupForTypePiece);
            RadioGroupSelectStylePiece.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(RadioGroup group, int checkedId) {
                    handleRadioButtonStyleChange(RadioGroupSelectStylePiece, radioButtonStyle);
                }
            });

             buttonEnd = (Button) findViewById(R.id.EndGameButton);
            buttonEnd.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    handleButtonClick();
                }
            });
        }
        catch (Exception e) {
            Utilities.messageBox("Failed to create game", e.getMessage());
        }

    }

    public void handleRadioButtonStyleChange(RadioGroup radioGroup, View view) {

        int selectedId = radioGroup.getCheckedRadioButtonId();
        // find the radiobutton by returned id
        view = (RadioButton) findViewById(selectedId);
        switch(selectedId) {
            case R.id.radioButtonStyle1:
                Game.style = "1";
                break;

            case R.id.radioButtonStyle2:
                Game.style = "2";
                break;
        }

        Display.drawFullBoard();
    }

    private void handleRadioButtonBoardColorChange(RadioGroup radioGroup, View view) {

        int selectedId = radioGroup.getCheckedRadioButtonId();
        // find the radiobutton by returned id
        radioButtonColor = (RadioButton) findViewById(selectedId);
        switch(selectedId) {
            case R.id.radioButtonRed:
                Display.blackSquareColor = ColorBoard.redSquareColor;
                Display.whiteSquareColor = ColorBoard.whiteSquareColor;
                break;

            case R.id.radioButtonBlue:
                Display.blackSquareColor = ColorBoard.blueSquareColor;
                Display.whiteSquareColor = ColorBoard.whiteSquareColor;
                break;

            case R.id.radioButtonWood:
                Display.blackSquareColor = ColorBoard.woodSquareColor;
                Display.whiteSquareColor = ColorBoard.whiteSquareColor;
                break;
        }

        Display.drawFullBoard();
    }

    private void handleButtonClick() {
        try {
            HttpRunner.runPostGameEnd();
        }
        catch (Exception e) {
            //e.printStackTrace();
            Utilities.messageBox("Error after pressing button", e.getMessage());
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        try {

        super.onWindowFocusChanged(hasFocus);
        board.requestLayout();
        if (!gameStarted) {
            //Game.initializeGame();
            //Utilities.messageBoxStartGame();
            gameStarted = true;
        }
        Display.drawFullBoard();
        Game.leftSpace = leftSpace.getWidth();

        //handleRadioChange();
        handleRadioButtonBoardColorChange(RadioGroupSelectColorBoard, radioButtonColor);
        handleRadioButtonStyleChange(RadioGroupSelectStylePiece, radioButtonStyle);

        }
        catch (Exception e) {
            Utilities.messageBox("Failed to change focus", e.getMessage());
        }
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();

        try {
            HttpRunner.runPostGameEnd();
        } catch (JSONException e) {
            e.printStackTrace();
        }

        Intent setIntent = new Intent(this,ActivityCreateGame.class);
        startActivity(setIntent);

    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
        try {
            HttpRunner.runPostGameEnd();

        } catch (JSONException e) {
            e.printStackTrace();
        }
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
            Utilities.messageBox("Error with view touch event", e.getMessage());
        }

    }

    private void handleRadioChange() {
        try {
            applyFilter(selectStyle);
            changeStyle( radioButtonID, radioButton);
            //Display.setBoardImg();
            Display.drawFullBoard();
        }
        catch (Exception e) {
            //e.printStackTrace();
            Utilities.messageBox("Error with radio change", e.getMessage());
        }
    }

    public static void applyFilter(RadioGroup selectStyle) {
        for (int i = 0 ; i < selectStyle.getChildCount() ; i++) {
            if ( i % 2 == 0) {
                View tmpRadio = selectStyle.getChildAt(i);
                //Log.d("", selectStyle.getChildCount()+"");
                tmpRadio.getBackground().clearColorFilter();
            }
        }
        int radioButtonID = selectStyle.getCheckedRadioButtonId();
        View radioButton = selectStyle.findViewById(radioButtonID);
        radioButton.getBackground().setColorFilter(android.graphics.Color.GRAY, PorterDuff.Mode.MULTIPLY);
    }

    public static void changeStyle(int radioButtonID, View radioButton) {

        if (index % 2 == 0) {
            Game.style = "1";
        }
        else {
            Game.style = "2";
        }
        if (index / 2 == 0) {
           // Display.blackSquareColor = ColorBoard.redSquareColor;
           // Display.whiteSquareColor = ColorBoard.whiteSquareColor;
        }
        else if (index / 2 == 1) {
         //   Display.blackSquareColor = ColorBoard.blueSquareColor;
           // Display.whiteSquareColor = ColorBoard.whiteSquareColor;
        }
        else {
           // Display.blackSquareColor = ColorBoard.woodSquareColor;
            //Display.whiteSquareColor = ColorBoard.whiteSquareColor;
        }
    }

}









