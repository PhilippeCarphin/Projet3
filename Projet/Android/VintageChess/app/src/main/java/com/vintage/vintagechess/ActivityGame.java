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
import android.widget.TextClock;
import android.widget.TextView;

import org.json.JSONException;

import java.util.ArrayList;
import java.util.LinkedList;

public class ActivityGame extends AppCompatActivity {


    //chessboard views
    public ImageView board;
    public ImageView motionlessPieces;
    public ImageView movingPiece;

    private TextView textViewPlayer1Name, textViewPlayer2Name;
    private TextClock textClockPlayer1, textClockPlayer2;
    private TextView textViewState;
    private TextView textViewRoundNumber;
    private TextView textViewWhoseTurn;
    private TextView textViewMoveNumber;
    private TextView textViewLocation;

    private RadioGroup radioGroupBoardColor;
    private RadioGroup radioGroupPieceStyle;

    private Button buttonEndGame;

    private Game game;

    RadioGroup selectStyle;

    @Override
    protected  void onResume() {
        super.onResume();
        Utilities.currentActivity = this;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        try {
            game = new Game(this);
            Utilities.currentActivity = this;

            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_game);

            setAllViews();

            Utilities.currentActivity = this;


        }
        catch (Exception e) {
            Utilities.messageBox("Failed to create game", e.getMessage());
        }
    }

    private void setAllViews() {
        setImageViews();
        setTextViews();
        setButtons();
    }

    private void setImageViews() {
        board = (ImageView) findViewById(R.id.imageViewBoard);
        motionlessPieces = (ImageView) findViewById(R.id.imageViewMotionlessPieces);
        movingPiece = (ImageView) findViewById(R.id.imageViewMovingPieces);


        board.setImageResource(android.R.color.transparent);
        motionlessPieces.setImageResource(android.R.color.transparent);
        movingPiece.setImageResource(android.R.color.transparent);


        board.setOnTouchListener(new View.OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
                handleTouch(event);
                return true;
            }
        });
    }

    private void setTextViews() {
        textViewState = (TextView)  findViewById(R.id.State);
        textViewPlayer1Name = (TextView) findViewById(R.id.Player1Name);
        textViewPlayer2Name = (TextView) findViewById(R.id.Player2Name);
        textViewLocation = (TextView) findViewById(R.id.LocationText);
        textClockPlayer1 = (TextClock) findViewById(R.id.TimePlayer1);
        textClockPlayer2 = (TextClock) findViewById(R.id.TimePlayer2);
        textViewRoundNumber = (TextView) findViewById(R.id.RoundText);
        textViewWhoseTurn = (TextView) findViewById(R.id.CurrentPlayer);
        textViewMoveNumber = (TextView) findViewById(R.id.MoveNumber);

        textViewPlayer1Name.setText(GameConfig.playerName1);
        textViewPlayer2Name.setText(GameConfig.playerName2);
        textViewLocation.setText(GameConfig.location);
        textViewRoundNumber.setText("Round : 0" );
    }

    private void setButtons() {
        radioGroupBoardColor = (RadioGroup) findViewById(R.id.RadioGroupBoardColor);
        radioGroupPieceStyle = (RadioGroup) findViewById(R.id.radioGroupStyle);
        buttonEndGame = (Button) findViewById(R.id.EndGameButton);

        radioGroupBoardColor.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                handleRadioButtonBoardColorChange();
            }
        });
        radioGroupPieceStyle.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                handleRadioButtonStyleChange();
            }
        });
        buttonEndGame.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                handleButtonEndClick();
            }
        });
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

            int backColor = r.isChecked() ? getResources().getColor(R.color.colorAccent) : Color.TRANSPARENT;
            setRadioButtonImage(r, backColor, resourceNames[i][0], resourceNames[i][1]);
            i++;
        }

        switch(selectedId) {
            case R.id.radioButtonStyle1:
                GameConfig.style = "1";
                break;

            case R.id.radioButtonStyle2:
                GameConfig.style = "2";
                break;
        }

        game.drawFullBoard();
    }

    private void handleRadioButtonBoardColorChange() {

        int selectedId = radioGroupBoardColor.getCheckedRadioButtonId();
        // find the radiobutton by returned id

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

        LinkedList<RadioButton> listOfRadioButtons = getRadioGroupButtons(radioGroupBoardColor);
        int i = 0;
        for (RadioButton r : listOfRadioButtons) {

            int backColor = r.isChecked() ? getResources().getColor(R.color.colorAccent) : Color.TRANSPARENT;
            int frontColor = i == 0 ? BoardColors.greenSquareColor : (i == 1 ? BoardColors.blueSquareColor : BoardColors.redSquareColor);
            setRadioButtonImage(r, backColor, frontColor);
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
        if (!Game.isStarted) {
            Utilities.messageBoxStartGame();
            Game.isStarted = true;
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
            Utilities.currentActivity = null;

        } catch (JSONException e) {
            e.printStackTrace();
            Utilities.messageBox("Error after destroying view", e.getMessage());
        }
        System.gc();
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
        textViewMoveNumber.setText(text);
    }

    public void setWhoseTurn(String turn) {
        Game.isWhiteTurn = turn.equals("1");
        this.textViewWhoseTurn.setText(Game.isWhiteTurn ? "White" : "Black");
    }

    private void setRadioButtonImage(RadioButton radioButton, int backColor, int frontColor) {
        Bitmap imageAndroid = Bitmap.createBitmap(100, 100, Bitmap.Config.ARGB_8888);
        Canvas tempCanvas = new Canvas(imageAndroid);
        Paint p = new Paint();
        p.setColor(backColor);
        tempCanvas.drawRect(0,0,tempCanvas.getHeight(), tempCanvas.getWidth(), p);
        p.setColor(frontColor);
        tempCanvas.drawRect((int)(tempCanvas.getHeight()*0.1),(int)(tempCanvas.getHeight()*0.1), (int)(tempCanvas.getHeight()*0.9), (int)(tempCanvas.getHeight()*0.9), p);
        radioButton.setBackground(new BitmapDrawable(this.getResources(), imageAndroid));
    }

    private void setRadioButtonImage(RadioButton radioButton, int backColor, String frontResource1, String frontResource2) {
        Bitmap imageAndroid = Bitmap.createBitmap(200, 100, Bitmap.Config.ARGB_8888);
        Canvas tempCanvas = new Canvas(imageAndroid);
        Paint p = new Paint();
        p.setColor(backColor);
        tempCanvas.drawRect(0,0,tempCanvas.getWidth(), tempCanvas.getHeight(), p);
        String[] resourceNames = new String[] {frontResource1, frontResource2};
        for (int i = 0 ; i < 2 ; i++) {
            Bitmap b = Piece.getResourceBitmap(resourceNames[i], tempCanvas.getHeight());
            tempCanvas.drawBitmap(b, i*tempCanvas.getHeight(), 0, null);
        }

        radioButton.setBackground(new BitmapDrawable(this.getResources(), imageAndroid));
    }

    private LinkedList<RadioButton> getRadioGroupButtons(RadioGroup radioGroup) {
        int count = radioGroup.getChildCount();
        LinkedList<RadioButton> listOfRadioButtons = new LinkedList<RadioButton>();
        for (int i=0;i<count;i++) {
            View o = radioGroup.getChildAt(i);
            if (o instanceof RadioButton) {
                listOfRadioButtons.add((RadioButton)o);
            }
        }
        return listOfRadioButtons;
    }

}









