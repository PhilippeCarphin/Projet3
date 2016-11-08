package com.vintage.vintagechess;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.widget.NumberPicker;
import android.widget.TimePicker;

/**
 * Created by User on 2016-10-12.
 */

public class Utilities {
    public static void messageBox(String title, String content, AppCompatActivity activity) {
        final AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(activity);
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

    public static String getTimePickerString(TimePicker tP) {
        return Integer.valueOf(tP.getCurrentHour()).toString() + ":" + Integer.valueOf(tP.getCurrentMinute()).toString()+ ":00";
    }

    public static void setNumberPickerTime(NumberPicker numberPicker, int minValue, int maxValue, int currentValue)
    {
        numberPicker.setMinValue(minValue);
        numberPicker.setMaxValue(maxValue);
        numberPicker.setValue(currentValue);
    }
}
