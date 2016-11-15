package com.vintage.vintagechess;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class ActivityJoinGame extends AppCompatActivity {

    @Override
    protected  void onResume() {
        super.onResume();
        Utilities.currentActivity = this;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_join_game);
    }
}
