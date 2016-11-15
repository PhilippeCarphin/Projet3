package com.vintage.vintagechess;

import android.graphics.Point;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.LinkedList;

/**
 * Created by Sabrina on 02/12/2016.
 */

public class REST {

    private static ActivityCreateGame activityCreateGame;// = new ActivityCreateGame();
    private static ActivityGame activityGame;

    public static String postNewGame() throws JSONException {


        JSONObject outerObject = new JSONObject();
        JSONObject innerObject = new JSONObject();

        innerObject.put("time",CreateGameInfo.normalTime);
        innerObject.put("increment",CreateGameInfo.timePerPlay);
        innerObject.put("limit",CreateGameInfo.allowedTurns);
        innerObject.put("overtime",CreateGameInfo.allowedTurns);
        innerObject.put("overtimeIncrement",CreateGameInfo.allowedTurns);
        outerObject.put("player1",CreateGameInfo.playerName1);
        outerObject.put("player2", CreateGameInfo.playerName2);
        outerObject.put("round",CreateGameInfo.round);
        outerObject.put("location", CreateGameInfo.location);
        outerObject.put("secret_code",CreateGameInfo.password);
        outerObject.put("twoTablet", CreateGameInfo.oneTablet);
        outerObject.put("enPassant", CreateGameInfo.enPassantOption);
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


    public static void updateGameFromStatusSummary(String Status) throws JSONException {

        JSONObject jsonObject = new JSONObject(Status);
        String turn = jsonObject.getString("turn");
        String moveNo = jsonObject.getString("moveNo");
        String lastMove = jsonObject.getString("lastMove");
        String state = jsonObject.getString("state");

        Log.d("turn : ", String.valueOf(turn));
        Log.d("moveNo : ", String.valueOf(moveNo));
        Log.d("lastMove : ",lastMove);
        Log.d("state : ",state);

        Display.updateGameFromStatusSummary(turn, moveNo, lastMove, state);

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


    public static LinkedList<Piece> getPiecesFromStatusBoard(JSONObject jsonObject) throws JSONException {


        LinkedList<Piece> newConfig = new LinkedList<Piece>();

        String[] types1 = new String[] {"king", "queen"};
        String[] types2 = new String[] {"bishop", "rook", "knight"};
        String[] types8 = new String[] {"pawn"};
        String[][] allTypes = new String[][]{types1, types2, types8};

        for (String type : types1) {
            for (int i = 0 ; i < 2 ; i++) {
                String search = type + (i+1);
                boolean isWhite = (i == 0);
                String position = jsonObject.getString(search);
                if (!position.equals("xx")) {
                    Point p = Utilities.getGridCoordinates(position);
                    newConfig.add(new Piece(type, p.x, p.y, isWhite));
                }
            }
        }
        for (String type : types2) {
            for (int i = 0 ; i < 2 ; i++) {
                String search = type + (i+1);
                boolean isWhite = (i == 0);
                for (int j = 0 ; j < 2 ; j++) {
                    String position = jsonObject.getString(search + (j == 0 ? "A" : "B"));
                    if (!position.equals("xx")) {
                        Point p = Utilities.getGridCoordinates(position);
                        newConfig.add(new Piece(type, p.x, p.y, isWhite));
                    }
                }
            }
        }

        for (String type : types8) {
            for (int i = 0 ; i < 2 ; i++) {
                String search = type + (i+1);
                boolean isWhite = (i == 0);
                JSONArray pawns = jsonObject.getJSONArray(search);
                for (int j = 0 ; j < 8 ; j++) {
                    String position = pawns.getString(j);
                    if (!position.equals("xx")) {
                        Point p = Utilities.getGridCoordinates(position);
                        newConfig.add(new Piece(type, p.x, p.y, isWhite));
                    }
                }
            }
        }
        return newConfig;
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
