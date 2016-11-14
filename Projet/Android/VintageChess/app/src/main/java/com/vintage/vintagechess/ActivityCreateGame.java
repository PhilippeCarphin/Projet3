package com.vintage.vintagechess;

import android.content.Intent;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.NumberPicker;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Switch;
import android.widget.TimePicker;

public class ActivityCreateGame extends AppCompatActivity {

    private RadioGroup radioSelect;
    private RadioButton radioButton1, radioButton2;

    private EditText editTextLocation, editTextPassword, editTextPlayer1Name, editTextPlayer2Name, editTextRound;

    private TimePicker timePickerMain,timePickerOverTime;
    private Switch enPassant;

    private NumberPicker numberPickerTurnsBeforedraw, numberPickerOverTimePerPlay, numberPickerTimePerPlay;

    private Button buttonCreateGame;
    public static String location;
    public static boolean oneTablet;
    public static String password;
    public static String playerName1;
    public static String playerName2;
    public static String round;
    public static int normalTime;
    public static int overTime;
    public static int allowedTurns;
    public static int timePerPlay;
    public static int overTimeIncr;
    public static boolean enPassantOption;
    View radioButtonStyle;
    View radioButtonColor;
    RadioGroup RadioGroupModeTablette;
    RadioGroup RadioGroupSelectColorBoard;
    RadioGroup RadioGroupSelectStylePiece;

    public static REST rest;


    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected  void onResume() {
        super.onResume();
        Utilities.currentActivity = this;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_game);

        Utilities.hideKeyPad(this);

        ActivityGame.radioButtonStyle1 = (RadioButton) findViewById(R.id.radioButtonStyle1);
        ActivityGame.radioButtonStyle2 = (RadioButton) findViewById(R.id.radioButtonStyle2);

        ActivityGame.radioButtonColorRed = (RadioButton) findViewById(R.id.radioButtonRed);
        ActivityGame.radioButtonColorblue = (RadioButton) findViewById(R.id.radioButtonBlue);
        ActivityGame.radioButtonColorGreen = (RadioButton) findViewById(R.id.radioButtonGreen);

        radioButton1 = (RadioButton) findViewById(R.id.radioButton1);
        radioButton2 = (RadioButton) findViewById(R.id.radioButton2);
        radioSelect = (RadioGroup) findViewById(R.id.radioGroupMode);
        radioSelect.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                handleRadioChange();
            }
        });

        editTextLocation = (EditText) findViewById(R.id.editTextLocation);
        editTextPassword = (EditText) findViewById(R.id.editTextPassword);
        editTextPassword.setEnabled(true);
        editTextRound = (EditText) findViewById(R.id.editTextRound);
        editTextPlayer1Name = (EditText) findViewById(R.id.editTextNamePlayer1);

        editTextPlayer2Name = (EditText) findViewById(R.id.editTextNamePlayer2);

        timePickerMain = (TimePicker) findViewById(R.id.timePickerTimePerPlayer);
        timePickerOverTime = (TimePicker) findViewById(R.id.timePickerOverTime);

        timePickerMain.setIs24HourView(true);
        timePickerOverTime.setIs24HourView(true);
        timePickerMain.setCurrentHour(1);
        timePickerMain.setCurrentMinute(0);
        timePickerOverTime.setCurrentHour(0);
        timePickerOverTime.setCurrentMinute(15);


        numberPickerOverTimePerPlay = (NumberPicker) findViewById(R.id.numberPickerOverTimePerPlay);
        Utilities.setNumberPickerTime(numberPickerOverTimePerPlay, 1, 200, 30);

        numberPickerTimePerPlay = (NumberPicker) findViewById(R.id.numberPickerTimePerPlay);
        Utilities.setNumberPickerTime(numberPickerTimePerPlay, 1, 200, 30);

        numberPickerTurnsBeforedraw = (NumberPicker) findViewById(R.id.numberPicker);
        Utilities.setNumberPickerTime(numberPickerTurnsBeforedraw, 1, 200, 40);

        enPassant = (Switch) findViewById(R.id.switchEnPassant);

        buttonCreateGame = (Button) findViewById(R.id.buttonCreateGame);
        buttonCreateGame.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                handleButtonClick();
            }
        });

        Display.blackSquareColor = BoardColors.redSquareColor;
        Display.lightSquareColor = BoardColors.whiteSquareColor;
        Game.style = "2";

        RadioGroupModeTablette = (RadioGroup) findViewById(R.id.radioGroupMode);
        RadioGroupModeTablette.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                handleRadioChange();
            }
        });

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

        Callbacks.activityCreateGame = this;
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
    }

    public void handleRadioChange() {
        //editTextPassword.setEnabled(radioButton2.isChecked());
        editTextPlayer2Name.setEnabled(radioButton1.isChecked());

    }

   private void handleRadioButtonBoardColorChange(RadioGroup radioGroup, View view) {

        int selectedId = radioGroup.getCheckedRadioButtonId();
        // find the radiobutton by returned id
        radioButtonColor = (RadioButton) findViewById(selectedId);
        switch(selectedId) {
            case R.id.radioButtonRed:
                Display.blackSquareColor = BoardColors.redSquareColor;
                Display.lightSquareColor = BoardColors.whiteSquareColor;
                break;

            case R.id.radioButtonBlue:
                Display.blackSquareColor = BoardColors.blueSquareColor;
                Display.lightSquareColor = BoardColors.whiteSquareColor;
                break;

            case R.id.radioButtonGreen:
                Display.blackSquareColor = BoardColors.greenSquareColor;
                Display.lightSquareColor = BoardColors.whiteSquareColor;
                break;
        }
    }

    private void handleButtonClick() {
        try {
            oneTablet = radioButton2.isChecked();
            enPassantOption = enPassant.isChecked();
            location = editTextLocation.getText().toString();
            playerName1 = editTextPlayer1Name.getText().toString();
            playerName2 = editTextPlayer2Name.getText().toString();
            password = editTextPassword.getText().toString();
            normalTime = timePickerMain.getCurrentMinute() + timePickerMain.getCurrentHour() * 60;
            timePerPlay = numberPickerTimePerPlay.getValue();
            overTime = timePickerOverTime.getCurrentMinute() + timePickerOverTime.getCurrentHour() * 60;
            overTimeIncr = numberPickerTimePerPlay.getValue();
            allowedTurns = numberPickerTurnsBeforedraw.getValue();
            round = editTextRound.getText().toString();

            //handle errors in entries
            if (radioButton2.isChecked()) {
                throw new Exception("The two tablet functionality is not implemented yet!");
            }
            HttpRunner.runPostNewGame(Callbacks.getPostNewGameCallback(),null);

        }
        catch (Exception e) {
            //e.printStackTrace();
            Utilities.messageBox("Error after pressing button", e.getMessage());
        }
    }

    public void openGame() {
        Intent intent = new Intent(buttonCreateGame.getContext(), ActivityGame.class);
        startActivity(intent);
    }
}
