package com.vintage.vintagechess;

import android.content.Context;
import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by Sabrina on 02/12/2016.
 */

public class REST {

    private static ActivityCreateGame activityGame;// = new ActivityCreateGame();

    public static String postNewGame() throws JSONException {


        JSONObject outerObject = new JSONObject();
        JSONObject innerObject = new JSONObject();

        innerObject.put("time",activityGame.normalTime);
        innerObject.put("increment",activityGame.timePerPlay);
        innerObject.put("limit",activityGame.allowedTurns);
        innerObject.put("overtime",activityGame.allowedTurns);
        innerObject.put("overtimeIncrement",activityGame.allowedTurns);

        outerObject.put("player1",activityGame.playerName1);
        outerObject.put("player2","TODO");
        outerObject.put("round","TODO");
        outerObject.put("location", activityGame.location);
        outerObject.put("secret_code","helloworld");//activityGame.password);
        outerObject.put("twoTablet", activityGame.oneTablet);
        outerObject.put("enPassant", activityGame.enPassantOption);
        outerObject.put("timerFormat", innerObject);

        //Log.d("NewGame : ", String.valueOf(outerObject));

        return String.valueOf(outerObject);
    }


    public static void handleMoveResponse(String response)
            throws JSONException {

        Log.d("Response", response);
        JSONObject jsonObject = new JSONObject(response);
        String pieceEleminated = jsonObject.getString("pieceEliminated");
        String promotion = jsonObject.getString("promotion");
        String state = jsonObject.getString("state");

        Game.handleMoveOk(pieceEleminated, promotion, state);


    }

    public static void getTime(String Time)
            throws JSONException {


        JSONObject jsonObject = new JSONObject(Time);
        Integer time = jsonObject.getInt("time");
        Integer overtime = jsonObject.getInt("overtime");

        Log.d("TIME : ", String.valueOf(time));
        Log.d("overtime : ", String.valueOf(overtime));

    }


    public static void getStatusSummary(String Status) throws JSONException {

        JSONObject jsonObject = new JSONObject(Status);
        Integer turn = jsonObject.getInt("turn");
        Integer moveNo = jsonObject.getInt("moveNo");
        String lastMove = jsonObject.getString("lastMove");
        String state = jsonObject.getString("state");

        Log.d("turn : ", String.valueOf(turn));
        Log.d("moveNo : ", String.valueOf(moveNo));
        Log.d("lastMove : ",lastMove);
        Log.d("state : ",state);

    }

    public static String statusBoard() throws JSONException {

        JSONObject innerObject = new JSONObject();

        innerObject.put("turn","");
        innerObject.put("move_no","");
        innerObject.put("king1","");
        // "TOUTES LES PIECE" pas fini


        Log.d("StatusBoard : ", String.valueOf(innerObject));

        return String.valueOf(innerObject);
    }


    /*--------------LIVRABLE 2 ----------------------------------------*/
    public static void postPromotePiece()
            throws JSONException {
        // TODO livrale2
    }

    // IMPORTANT POUR JOUEUR 2 livrale2
    public static void getGameDetails(String GameDetails) throws JSONException {

        JSONObject jsonObject = new JSONObject(GameDetails);
        String player1 = jsonObject.getString("player1");
        String player2 = jsonObject.getString("player2");
        String round = jsonObject.getString("round");
        String location = jsonObject.getString("location");
        String enPassant = jsonObject.getString("enPassant");
        JSONObject timeFormat = jsonObject.getJSONObject("timeFormat");
        String time = timeFormat.getString("time");
        String increment = timeFormat.getString("increment");
        String limit = timeFormat.getString("limit");
        String overtime = timeFormat.getString("overtime");
        String overtimeIncrement = timeFormat.getString("overtimeIncrement");

        Log.d("player1 : ", String.valueOf(player1));
        Log.d("player2 : ", String.valueOf(player2));
        Log.d("round : ",round);
        Log.d("location : ",location);

    }

}
