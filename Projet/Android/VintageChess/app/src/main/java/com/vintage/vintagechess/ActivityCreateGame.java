package com.vintage.vintagechess;

import android.app.Activity;
import android.app.TimePickerDialog;
import android.content.Intent;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.NumberPicker;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Switch;
import android.widget.TimePicker;

import org.json.JSONException;

public class ActivityCreateGame extends AppCompatActivity {

    private RadioGroup radioSelect;
    private RadioButton radioButton1, radioButton2;

    private EditText editTextLocation, editTextPassword, editTextPlayer1Name, editTextPlayer2Name;

    private TimePicker timePickerMain,timePickerOverTime;
    private Switch enPassant;

    private NumberPicker numberPickerTurnsBeforedraw, numberPickerOverTimePerPlay, numberPickerTimePerPlay;

    private Button buttonCreateGame;
    public static String location;
    public static boolean oneTablet;
    public static String password;
    public static String playerName1;
    public static String normalTime;
    public static String overTime;
    public static String allowedTurns;
    public static String timePerPlay;
    public static String overTimeIncr;
    public static boolean enPassantOption;

    static REST rest;

    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_game);

        Utilities.hideKeyPad(this);

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
        editTextPassword.setEnabled(false);
        editTextPlayer1Name = (EditText) findViewById(R.id.editTextNamePlayer1);

        editTextPlayer2Name = (EditText) findViewById(R.id.editTextNamePlayer2);
        //editTextPlayer2Name.setEnabled(true);

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
                handleClick(v);
            }
        });

    }

    private void handleRadioChange() {
            editTextPassword.setEnabled(radioButton2.isChecked());
        editTextPlayer2Name.setEnabled(radioButton1.isChecked());
    }

    @RequiresApi(api = Build.VERSION_CODES.M) /* suggéré par android studio */
    private void handleClick(View v) {
        try {
            oneTablet = radioButton2.isChecked();
            enPassantOption = enPassant.isChecked();
            location = editTextLocation.getText().toString();
            playerName1 = editTextPlayer1Name.getText().toString();
            password = editTextPassword.getText().toString();
            normalTime = Utilities.getTimePickerString(timePickerMain);
            timePerPlay = Integer.valueOf(numberPickerTimePerPlay.getValue()).toString();
            overTime = Utilities.getTimePickerString(timePickerOverTime);
            overTimeIncr = Integer.valueOf(numberPickerTimePerPlay.getValue()).toString();
            allowedTurns = Integer.valueOf(numberPickerTurnsBeforedraw.getValue()).toString();

            //handle errors in entries
            if (radioButton2.isChecked()) {
                throw new Exception("The two tablet functionality is not implemented yet!");
            }

            HttpRunner.runPostNewGame();

            /*****REST TEST*****/
            String newGame = rest.postNewGame();
            rest.getGameDetails(newGame);



            Intent intent = new Intent(v.getContext(), ActivityGame.class);
            startActivity(intent);
        }
        catch (Exception e) {
            Utilities.messageBox("ERROR", e.getMessage(), this);
            e.printStackTrace();
        }
    }
}
