package com.vintage.vintagechess;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;

public class ActivityMenu extends AppCompatActivity {

    private Button buttonPlay;
    private EditText editText;

    @Override
    protected  void onResume() {
        super.onResume();
        Utilities.currentActivity = this;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);

        Utilities.hideKeyPad(this);

        buttonPlay = (Button) findViewById(R.id.buttonPlay);
        buttonPlay.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                handleClick(v);
            }
        });

        editText = (EditText) findViewById(R.id.editTextIp);
    }

    private void handleClick(View v) {
        try {
            Game.ip = editText.getText().toString();
            boolean existingGame = false;
            //todo: handle http to know if existing game or not. If one already exists and contains
            //two players, throw an exception.

            Class activityClass = existingGame ? ActivityJoinGame.class : ActivityCreateGame.class;
            Intent intent = new Intent(v.getContext(), activityClass);
            startActivity(intent);
        }
        catch(Exception e) {
            Utilities.messageBox("ERROR", e.getMessage());
            e.printStackTrace();
        }
    }
}

