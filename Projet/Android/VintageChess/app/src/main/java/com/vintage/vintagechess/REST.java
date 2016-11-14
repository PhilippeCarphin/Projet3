package com.vintage.vintagechess;

import android.content.Context;
import android.graphics.Point;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by Sabrina on 02/12/2016.
 */

public class REST {

    private static ActivityCreateGame activityCreateGame;// = new ActivityCreateGame();
    private static ActivityGame activityGame;

    public static String postNewGame() throws JSONException {


        JSONObject outerObject = new JSONObject();
        JSONObject innerObject = new JSONObject();

        innerObject.put("time",activityCreateGame.normalTime);
        innerObject.put("increment",activityCreateGame.timePerPlay);
        innerObject.put("limit",activityCreateGame.allowedTurns);
        innerObject.put("overtime",activityCreateGame.allowedTurns);
        innerObject.put("overtimeIncrement",activityCreateGame.allowedTurns);

        outerObject.put("player1",activityCreateGame.playerName1);
        outerObject.put("player2","TODO");
        outerObject.put("round","TODO");
        outerObject.put("location", activityCreateGame.location);
        outerObject.put("secret_code",activityCreateGame.password);
        outerObject.put("twoTablet", activityCreateGame.oneTablet);
        outerObject.put("enPassant", activityCreateGame.enPassantOption);
        outerObject.put("timerFormat", innerObject);

        //Log.d("NewGame : ", String.valueOf(outerObject));

        return String.valueOf(outerObject);
    }

    public static void handleMoveResponse(String response)
            throws JSONException {
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
        String turn = jsonObject.getString("turn");
        String moveNo = jsonObject.getString("moveNo");
        String lastMove = jsonObject.getString("lastMove");
        String state = jsonObject.getString("state");

        Log.d("turn : ", String.valueOf(turn));
        Log.d("moveNo : ", String.valueOf(moveNo));
        Log.d("lastMove : ",lastMove);
        Log.d("state : ",state);

        activityGame.statusSammarry(turn, moveNo, lastMove, state);

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


    public static String getStatusBoard(String Status) throws JSONException {

        String positionInBoard = "";

        JSONObject jsonObject = new JSONObject(Status);
        JSONArray pawn1 = jsonObject.getJSONArray("pawn1");
        JSONArray pawn2 = jsonObject.getJSONArray("pawn2");
        String turn = jsonObject.getString("turn");
        String moveNo = jsonObject.getString("move_no");

        int index1 = 0;
        int index2 = 0;

        int [] verticals = {0, 1, 6, 7};
        for(int i= 0; i< 8; i++)
        {
            for(int j : verticals)
            {
                if(Game.pieces[i][j].type_.equals("pawn1"))
                {
                    //Log.d(Game.pieces[i][j].type_, (String) pawn1.get(index1++));

                }else if( Game.pieces[i][j].type_.equals("pawn2"))
                {
                    // Log.d(Game.pieces[i][j].type_, (String) pawn1.get(index2++));
                }
                else
                {
                    positionInBoard = jsonObject.getString(Game.pieces[i][j].type_);
                    //Log.d(Game.pieces[i][j].type_, positionInBoard);
                }

                // int x =  Integer.valueOf(positionInBoard.charAt(0)) - 97; // 97 cest le a dans la table ascii
                // int y = 8 - Integer.valueOf(positionInBoard.charAt(1)) + 48; // 48 cest la 1 dans la table ascii
                // SETER
                //Game.pieces[i][j].p_ = new Point(x,y);
            }
        }

        /* TO DEBUG
        Log.d("turn", turn);
        Log.d("moveNo", moveNo);
        Log.d("pawn1", String.valueOf(pawn1));
        Log.d("pawn2", String.valueOf(pawn2));*/
        return String.valueOf(jsonObject);
    }

    public static String postStatusBoard() throws JSONException {

        JSONObject innerObject = new JSONObject();

        JSONArray listOfPawn1Position = new JSONArray();
        JSONArray listOfPawn2Position = new JSONArray();
        innerObject.put("turn"," ");
        innerObject.put("move_no"," ");
        int [] verticals = {0, 1, 6, 7};
        for(int i= 0; i< 8; i++)
        {
            for(int j : verticals)
            {
                Point point = Game.pieces[i][j].p_;
                String x =  String.valueOf((char)((char)point.x + 97));
                String y = Integer.toString(point.y);

                Log.d("x_Y : ", x+y);
                if(Game.pieces[i][j].type_.contains("pawn1"))
                {
                    listOfPawn1Position.put(x+y);

                } else if(Game.pieces[i][j].type_.contains("pawn2"))
                {
                    listOfPawn2Position.put(x+y);
                }
                else
                {
                    innerObject.put(Game.pieces[i][j].type_, x+y);
                }
            }

            innerObject.put("pawn1", listOfPawn1Position);
            innerObject.put("pawn2", listOfPawn2Position);
        }

        // TO DEBUG
        //Log.d("StatusBoard : ", String.valueOf(innerObject));

        return String.valueOf(innerObject);
    }
}
