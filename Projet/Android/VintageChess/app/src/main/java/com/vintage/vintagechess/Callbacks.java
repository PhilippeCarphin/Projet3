package com.vintage.vintagechess;
import android.support.v7.app.AppCompatActivity;
import org.json.JSONException;
import android.util.Log;


/**
 * Created by User on 2016-11-10.
 */

public class Callbacks {


    public static ActivityCreateGame activityCreateGame;


    public static RequestCallback getPostNewGameCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {
                try {
                    Log.d("response callback ", "\n\n\n\n" +response +"\n\n\n\n");
                    activityCreateGame.openGame();
                }
                catch (Exception e) {
                    e.printStackTrace();
                    Utilities.messageBox("Error while creating a new game", e.getMessage(), activityCreateGame);

                }
            }
        };
        return ret;
    }

    public static RequestCallback getPostGameStartCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

            }
        };
        return ret;
    }

    public static RequestCallback getPostGameEndCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

            }
        };
        return ret;
    }

    public static RequestCallback getPostMoveCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) throws JSONException {
                try {

                    Game.handleMoveOk();

                }
                catch (Exception ex) {
                    Game.handleMoveNotOk();
                    Utilities.messageBox("Erreur" + response , "Erreur" + response, (AppCompatActivity) Game.activityGame);
                }

            }
        };
        return ret;
    }

    public static RequestCallback getPostPromoteCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

                try {

                    //Game.promote

                }
                catch (Exception ex) {
                    Utilities.messageBox("Erreur" + response , "Erreur" + response, (AppCompatActivity) Game.activityGame);
                }


            }
        };
        return ret;
    }

    public static RequestCallback getGetTimeCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

            }
        };
        return ret;
    }

    public static RequestCallback getGetStatusSummaryCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {


            }
        };
        return ret;
    }

    public static RequestCallback getGetStatusBoardCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

            }
        };
        return ret;
    }

    public static RequestCallback getGetGameDetailsCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

            }
        };
        return ret;
    }



}
