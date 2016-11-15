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


    //private RadioGroup radioSelect;
    //private RadioButton radioButton1, radioButton2;

    private EditText editTextLocation, editTextPassword, editTextPlayer1Name, editTextPlayer2Name, editTextRound;

    private TimePicker timePickerMain,timePickerOverTime;
    private Switch enPassant;

    private NumberPicker numberPickerTurnsBeforedraw, numberPickerOverTimePerPlay, numberPickerTimePerPlay;

    private Button buttonCreateGame;

    View radioButtonStyle;
    View radioButtonColor;
    RadioGroup RadioGroupModeTablette;
    RadioGroup RadioGroupSelectColorBoard;
    RadioGroup RadioGroupSelectStylePiece;
    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected  void onResume() {
        super.onResume();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_game);

        Utilities.hideKeyPad(this);
        Utilities.currentActivity = this;
//        radioButton1 = (RadioButton) findViewById(R.id.radioButton1);
//        radioButton2 = (RadioButton) findViewById(R.id.radioButton2);
//        radioSelect = (RadioGroup) findViewById(R.id.radioGroupMode);
//        radioSelect.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
//            @Override
//            public void onCheckedChanged(RadioGroup group, int checkedId) {
//                handleRadioChange();
//            }
//        });

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



//        RadioGroupModeTablette = (RadioGroup) findViewById(R.id.radioGroupMode);
//        RadioGroupSelectColorBoard = (RadioGroup) findViewById(R.id.RadioGroupBoardColor);
//        RadioGroupSelectStylePiece = (RadioGroup) findViewById(R.id.RadioGroupForTypePiece);

    }
    @Override
    public void onDestroy() {
        super.onDestroy();
        Utilities.currentActivity = null;
        System.gc();
    }


    public void handleRadioChange() {
        //editTextPassword.setEnabled(radioButton2.isChecked());
        //editTextPlayer2Name.setEnabled(radioButton1.isChecked());

    }


    private void handleButtonClick() {
        try {
            GameConfig.oneTablet = true;//radioButton2.isChecked();
            GameConfig.enPassantOption = enPassant.isChecked();
            GameConfig.location = editTextLocation.getText().toString();
            GameConfig.playerName1 = editTextPlayer1Name.getText().toString();
            GameConfig.playerName2 = editTextPlayer2Name.getText().toString();
            GameConfig.password = editTextPassword.getText().toString();
            GameConfig.normalTime = timePickerMain.getCurrentMinute() + timePickerMain.getCurrentHour() * 60;
            GameConfig.timePerPlay = numberPickerTimePerPlay.getValue();
            GameConfig.overTime = timePickerOverTime.getCurrentMinute() + timePickerOverTime.getCurrentHour() * 60;
            GameConfig.overTimeIncr = numberPickerTimePerPlay.getValue();
            GameConfig.allowedTurns = numberPickerTurnsBeforedraw.getValue();
            GameConfig.round = editTextRound.getText().toString();

            //handle errors in entries
//            if (radioButton2.isChecked()) {
//                throw new Exception("The two tablet functionality is not implemented yet!");
//            }
            HttpRunner.runPostNewGame(this, null,Callbacks.getPostNewGameCallback(),GetUIOnFailNewGameCallBack());

        }
        catch (Exception e) {
            //e.printStackTrace();
            Utilities.messageBox("Error after pressing button", e.getMessage());
        }
    }

    RequestCallback GetUIOnFailNewGameCallBack(){
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        HttpRunner.runGetGameDetails(null, null, GetUIOnFailGetGameDetails());
                    }
                });
            }
        };
        return ret;
    }

    RequestCallback GetUIOnFailGetGameDetails() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response, ActivityGame activityGame, ActivityCreateGame activityCreateGame) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Utilities.messageBox("Game In Progress", "A game is in progress and you have the wrong password");
                    }
                });
            }
        };
        return ret;
    }

    public void openGame() {
        Intent intent = new Intent(Utilities.currentActivity, ActivityGame.class);
        startActivity(intent);
    }
}
