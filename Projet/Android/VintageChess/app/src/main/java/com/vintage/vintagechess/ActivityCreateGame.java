package com.vintage.vintagechess;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.NumberPicker;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TimePicker;

public class ActivityCreateGame extends AppCompatActivity {

    private RadioGroup radioSelect;
    private RadioButton radioButton1, radioButton2;

    private EditText editTextLocation, editTextPassword;

    private TimePicker timePickerMain, timePickerOverTime;

    private NumberPicker numberPickerTurnsBeforedraw;

    private Button buttonCreateGame;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_game);

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

        timePickerMain = (TimePicker) findViewById(R.id.timePickerTimePerPlayer);
        timePickerOverTime = (TimePicker) findViewById(R.id.timePickerOverTime);
        timePickerMain.setIs24HourView(true);
        timePickerOverTime.setIs24HourView(true);
        timePickerMain.setHour(1);
        timePickerMain.setMinute(0);
        timePickerOverTime.setHour(0);
        timePickerOverTime.setMinute(15);


        numberPickerTurnsBeforedraw = (NumberPicker) findViewById(R.id.numberPicker);
        numberPickerTurnsBeforedraw.setMinValue(1);
        numberPickerTurnsBeforedraw.setMaxValue(200);
        numberPickerTurnsBeforedraw.setValue(51);

        buttonCreateGame = (Button) findViewById(R.id.buttonCreateGame);
        buttonCreateGame.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                handleClick(v);
            }
        });



    }

    private void handleRadioChange() {
            editTextPassword.setEnabled(radioButton2.isChecked());
    }

    private void handleClick(View v) {
        try {
            boolean oneTablet = radioButton2.isChecked();
            String location = editTextLocation.getText().toString();
            String password = editTextPassword.getText().toString();
            String normalTime = Utilities.getTimePickerString(timePickerMain);
            String overTime = Utilities.getTimePickerString(timePickerOverTime);
            String allowedTurns = Integer.valueOf(numberPickerTurnsBeforedraw.getValue()).toString();

            //handle errors in entries
            if (radioButton2.isChecked()) {
                throw new Exception("The two tablet functionality is not implemented yet!");
            }


            Intent intent = new Intent(v.getContext(), ActivityGame.class);
            startActivity(intent);
        }
        catch (Exception e) {
            Utilities.messageBox("ERROR", e.getMessage(), this);
            e.printStackTrace();
        }
    }
}
