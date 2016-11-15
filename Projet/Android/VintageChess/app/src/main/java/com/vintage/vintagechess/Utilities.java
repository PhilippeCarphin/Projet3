package com.vintage.vintagechess;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.graphics.Point;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.WindowManager;
import android.widget.NumberPicker;
import android.widget.TimePicker;

import org.json.JSONException;

/**
 * Created by User on 2016-10-12.
 */

public class Utilities {
    public static ProgressDialog dialog;
    public static AppCompatActivity currentActivity;
    private static boolean debug = true;

    public static void messageBox(String title, String content) {

        final AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(currentActivity);
        dlgAlert.setMessage(content);
        dlgAlert.setTitle(title);
        dlgAlert.setPositiveButton("Ok",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        //dismiss the dialog
                    }
                });
        dlgAlert.setCancelable(false);
        dlgAlert.create().show();
    }

    public static void messageBoxStartGame(final ActivityGame activityGame) {

        final AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(currentActivity);
        dlgAlert.setMessage("The game will be started when you will have pressed OK");
        dlgAlert.setTitle("Start Game");
        dlgAlert.setPositiveButton("Ok",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        //dismiss the dialog
                        try {
                            HttpRunner.runPostGameStart(null, activityGame, null);
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                });
        dlgAlert.setCancelable(false);
        dlgAlert.create().show();
    }

    public static String getTimePickerString(TimePicker tP) {
        return Integer.valueOf(tP.getCurrentHour()).toString() + ":" + Integer.valueOf(tP.getCurrentMinute()).toString()+ ":00";
    }

    public static void setNumberPickerTime(NumberPicker numberPicker, int minValue, int maxValue, int currentValue)
    {
        numberPicker.setMinValue(minValue);
        numberPicker.setMaxValue(maxValue);
        numberPicker.setValue(currentValue);
    }

    public static void hideKeyPad(Activity act)
    {
        act.getWindow().setSoftInputMode(
                WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN
        );
    }

    public static void handleBadHttp(int code, String message) {
        String title = "Erreur " + code;
        messageBox(title, message);
    }

    public static Point getGridCoordinates(String boardCoordinates) {
        int x = boardCoordinates.charAt(0) - 97;
        int y = boardCoordinates.charAt(1) - 49;
        return new Point(x, y);
    }

    public static String getBoardCoordinates(Point p) {
        return (char)(p.x+97) +Integer.valueOf(p.y+1).toString();
    }

    public static void printStackTrace(Exception e) {
        if (debug) {
            e.printStackTrace();
        }
    }


}
