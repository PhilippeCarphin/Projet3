package com.vintage.vintagechess;

import android.support.v7.app.AppCompatActivity;

import org.json.JSONException;

/**
 * Created by User on 2016-11-10.
 */

public class Callbacks {

    static REST rest;
    ActivityGame activiteGame;

    public static RequestCallback getPostNewGameCallback() {
        RequestCallback ret = new RequestCallback() {
            @Override
            public void runResponse(String response) {

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

                    if (!response.contains("200")) throw new Exception("Response was not 200");
                   //Game.movingPiece

                }
                catch (Exception ex) {
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

                    if (!response.contains("200")) throw new Exception("Response was not 200");
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
