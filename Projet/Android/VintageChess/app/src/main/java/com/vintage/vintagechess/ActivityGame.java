package com.vintage.vintagechess;
import android.content.Intent;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
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

import java.util.ArrayList;

public class ActivityGame extends AppCompatActivity {

    ImageView board;
    ImageView motionlessPieces;
    ImageView movingPiece;
    Space leftSpace;
    TextView player1Name, state;
    TextView player2Name;
    TextClock timerPlaye1;
    TextClock timerPlaye2;
    TextView round;
    TextView turn;
    TextView moveNumber;
    TextView location;
    private Button buttonEnd;
    View radioButtonColor;
    private RadioGroup radioGroupBoardColor;
    private RadioGroup radioGroupPieceStyle;
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

            Game.activityGame = this;
            Utilities.currentActivity = this;
            gameStarted = false;



            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_game);
            round = (TextView) findViewById(R.id.textRound);
            turn = (TextView) findViewById(R.id.CurrentPlayer);
            moveNumber = (TextView) findViewById(R.id.MoveNumber);;
            location = (TextView) findViewById(R.id.LocationText);;

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


            Display.board = board;
            Display.motionlessPieces = motionlessPieces;
            Display.movingPiece = movingPiece;


            state = (TextView)  findViewById(R.id.State);
            player1Name = (TextView) findViewById(R.id.Player1Name);
            player2Name = (TextView) findViewById(R.id.Player2Name);
            location = (TextView) findViewById(R.id.LocationText);

            timerPlaye1 = (TextClock) findViewById(R.id.TimePlayer1);
            timerPlaye2 = (TextClock) findViewById(R.id.TimePlayer2);
            round = (TextView) findViewById(R.id.RoundText);
            turn = (TextView) findViewById(R.id.CurrentPlayer);
            moveNumber = (TextView) findViewById(R.id.MoveNumber);
            player1Name.setText(CreateGameInfo.playerName1);
            player2Name.setText(CreateGameInfo.playerName2);
            location.setText(CreateGameInfo.location);
            round.setText("Round : 0" );
             //
            // index = selectStyle.indexOfChild(radioButton)/2;

            radioGroupBoardColor = (RadioGroup) findViewById(R.id.RadioGroupBoardColor);
            int a = 0;
            radioGroupBoardColor.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(RadioGroup group, int checkedId) {
                    handleRadioButtonBoardColorChange();
                }
            });

            radioGroupPieceStyle = (RadioGroup) findViewById(R.id.radioGroupStyle);
            radioGroupPieceStyle.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(RadioGroup group, int checkedId) {
                    handleRadioButtonStyleChange();
                }
            });

             buttonEnd = (Button) findViewById(R.id.EndGameButton);
            buttonEnd.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    handleButtonEndClick();
                }
            });




            /*buttonQuit = (Button) findViewById(R.id.QuitGameButton);
            buttonQuit.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    //handleButtonQuitClick();
                }
            });*/
        }
        catch (Exception e) {
            Utilities.messageBox("Failed to create game", e.getMessage());
        }
    }

    public void handleRadioButtonStyleChange() {

        int selectedId = radioGroupPieceStyle.getCheckedRadioButtonId();
        // find the radiobutton by returned id
        RadioButton tempRadioButton = (RadioButton) findViewById(selectedId);

        int count = radioGroupPieceStyle.getChildCount();
        ArrayList<RadioButton> listOfRadioButtons = new ArrayList<RadioButton>();
        for (int i=0;i<count;i++) {
            View o = radioGroupPieceStyle.getChildAt(i);
            if (o instanceof RadioButton) {
                listOfRadioButtons.add((RadioButton)o);
            }
        }
        String[][] resourceNames = new String[][] {{"black_king_1", "white_queen_1"},{"black_king_2", "white_queen_2"}};
        int i = 0;
        for (RadioButton r : listOfRadioButtons) {
            Bitmap imageAndroid = Bitmap.createBitmap(200, 100, Bitmap.Config.ARGB_8888);
            Canvas tempCanvas = new Canvas(imageAndroid);
            int backColor = r.isChecked() ? getResources().getColor(R.color.colorAccent) : Color.TRANSPARENT;
            Paint p = new Paint();
            p.setColor(backColor);
            tempCanvas.drawRect(0,0,tempCanvas.getWidth(), tempCanvas.getWidth(), p);
            for (int j = 0 ; j < 2 ; j++) {
                Bitmap b = Piece.getResourceBitmap(resourceNames[i][j], tempCanvas.getHeight());
                tempCanvas.drawBitmap(b, j*tempCanvas.getHeight(), 0, null);
            }

            r.setBackground(new BitmapDrawable(this.getResources(), imageAndroid));
            i++;
        }

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

    private void handleRadioButtonBoardColorChange() {

        int selectedId = radioGroupBoardColor.getCheckedRadioButtonId();
        // find the radiobutton by returned id
        radioButtonColor = (RadioButton) findViewById(selectedId);

        switch(selectedId) {
            case R.id.radioButtonRed:
                Display.blackSquareColor = BoardColors.redSquareColor;
                break;

            case R.id.radioButtonBlue:
                Display.blackSquareColor = BoardColors.blueSquareColor;
                break;

            case R.id.radioButtonGreen:
                Display.blackSquareColor = BoardColors.greenSquareColor;
                break;
        }

        int count = radioGroupBoardColor.getChildCount();
        ArrayList<RadioButton> listOfRadioButtons = new ArrayList<RadioButton>();
        for (int i=0;i<count;i++) {
            View o = radioGroupBoardColor.getChildAt(i);
            if (o instanceof RadioButton) {
                listOfRadioButtons.add((RadioButton)o);
            }
        }
        int i = 0;
        for (RadioButton r : listOfRadioButtons) {
            r.setText("");
            Bitmap imageAndroid = Bitmap.createBitmap(100, 100, Bitmap.Config.ARGB_8888);
            Canvas tempCanvas = new Canvas(imageAndroid);
            int backColor = r.isChecked() ? getResources().getColor(R.color.colorAccent) : Color.TRANSPARENT;
            Paint p = new Paint();
            p.setColor(backColor);
            tempCanvas.drawRect(0,0,tempCanvas.getHeight(), tempCanvas.getWidth(), p);
            int frontColor = i == 0 ? BoardColors.greenSquareColor : (i == 1 ? BoardColors.blueSquareColor : BoardColors.redSquareColor);
            p.setColor(frontColor);
            tempCanvas.drawRect((int)(tempCanvas.getHeight()*0.1),(int)(tempCanvas.getHeight()*0.1), (int)(tempCanvas.getHeight()*0.9), (int)(tempCanvas.getHeight()*0.9), p);
            r.setBackground(new BitmapDrawable(this.getResources(), imageAndroid));
            i++;
        }

        Display.drawFullBoard();
    }

    private void handleButtonEndClick() {
        try {
            HttpRunner.runPostGameEnd(null);
            Intent setIntent = new Intent(this,ActivityMenu.class);
            startActivity(setIntent);


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
            Utilities.messageBoxStartGame();
            gameStarted = true;
        }
        Display.drawFullBoard();

        //handleRadioChange();
            handleRadioButtonBoardColorChange();
            handleRadioButtonStyleChange();

        }
        catch (Exception e) {
            Utilities.messageBox("Failed to change focus", e.getMessage());
        }
    }

    @Override
    public void onBackPressed() {
        //super.onBackPressed();

        try {
            HttpRunner.runPostGameEnd(null);
        } catch (JSONException e) {
            Utilities.messageBox("Error after having pressed back", e.getMessage());
        }


    }

    @Override
    public void onDestroy()
    {
        try {
            HttpRunner.runPostGameEnd(null);
            Game.activityGame = null;

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

    public void setMoveNumberText(String text) {
        moveNumber.setText(text);
    }

    public void setWhoseTurn(String turn) {
        Game.isWhiteTurn = turn.equals("1");
        this.turn.setText(Game.isWhiteTurn ? "White" : "Black");
    }

}









