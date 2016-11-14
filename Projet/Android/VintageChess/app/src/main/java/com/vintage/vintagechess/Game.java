package com.vintage.vintagechess;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Point;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;
import android.widget.ImageView;

import org.json.JSONException;

import java.util.ArrayList;
import java.util.LinkedList;

/**
 * Created by User on 2016-10-18.
 */

public class Game {

    public static String style ="1";
    private static Point downPos;
    private static Point lastPos;
    public static Piece currentPiece;
    public static String ip;
    public static boolean isWhiteTurn = true;


    public static Piece[][] pieces = new Piece[][] {
        {null,null,null,null,null,null,null,null},
        {null,null,null,null,null,null,null,null},
        {null,null,null,null,null,null,null,null},
        {null,null,null,null,null,null,null,null},
        {null,null,null,null,null,null,null,null},
        {null,null,null,null,null,null,null,null},
        {null,null,null,null,null,null,null,null},
        {null,null,null,null,null,null,null,null}};




    //sets the piece starting positions
    private static void initializePieces(LinkedList<Piece> newPieces) {
        clearPieces();
        for (Piece newPiece : newPieces) {
            pieces[newPiece.p_.x][newPiece.p_.y] = newPiece;
        }
    }

    //gets the piece at a certain position
    private static Piece getPieceOnCell(Point po) {
        return pieces[po.x][po.y];
    }

    //handler for when the user puts his finger on the chessboard
    public static void handleFingerDown(int xPix, int yPix) throws Exception {
        downPos = Display.getboardCoordinates(xPix, yPix);
        Log.d("down", downPos.x+" "+downPos.y);
        if (downPos != null) {
            currentPiece = getPieceOnCell(downPos);

        }
        if (currentPiece != null) {
            if (currentPiece.isWhite_ != isWhiteTurn) {
                currentPiece = null;
                downPos = null;
            }
            pieces[downPos.x][downPos.y] = null;
            Log.d("Selected piece", currentPiece.type_);
            Display.drawMotionlessPieces();
            Display.drawMovingPiece();
        }
    }

    //handler for when the user removes his finger from the chessboard
    public static void handleFingerUp() {
        //Log.d("up", lastPos.x+" "+lastPos.y);
        if (currentPiece != null && !downPos.equals(lastPos)) {
            Display.blockBoard();
            HttpRunner.runPostMove(isWhiteTurn, downPos.x, downPos.y, lastPos.x, lastPos.y);

        }
        else {

            handleMoveNotOk();
        }

    }

    //handler for when the user already has his finger on the screen and uses it
    public static void handleMove(int xPix, int yPix) throws Exception {
        //lastPos = getboardCoordinates(xPix, yPix);

        if (downPos == null || currentPiece == null ) {
            return;
        }
        Point p = Display.getboardCoordinates(xPix, yPix);
        if (!p.equals(lastPos)) {
            lastPos = p;
            currentPiece.p_ = lastPos;
            Display.drawMovingPiece();
        }
        //drawBoard();
    }

    //handles when the http returns that the asked move is ok
    public static void handleMoveOk(String pieceEleminated, String promotion, String state) throws JSONException {
        if (currentPiece != null) {
            if (!pieceEleminated.equals("xx")) {
                Point p = Utilities.getGridCoordinates(pieceEleminated);
                pieces[p.x][p.y] = null;
            }
            pieces[currentPiece.p_.x][currentPiece.p_.y] = currentPiece;
            //isWhiteTurn = !isWhiteTurn;
            Display.lastPos = downPos;
            Display.newPos = currentPiece.p_;
        }
        finishMove();
        HttpRunner.runGetStatusSummary();
    }


    public static void handleMoveNotOk() {
        if (currentPiece != null) {
            currentPiece.p_ = downPos;
            pieces[downPos.x][downPos.y] = currentPiece;
        }
        finishMove();
    }

    private static void finishMove() {
        downPos = null;
        lastPos = null;
        currentPiece = null;
        Display.drawFullBoard();
        Display.drawMovingPiece();
        Display.unBlockBoard();
    }

    public static void recoverFromError() {
        handleMoveNotOk();
    }

    public static void setConfig(LinkedList<Piece> newPieces) {
        clearPieces();
        for (Piece p : newPieces) {
            pieces[p.p_.x][p.p_.y] = p;
        }
        Display.drawMotionlessPieces();
    }

    public static void clearPieces() {
            pieces =  new Piece[][] {
                    {null,null,null,null,null,null,null,null},
                    {null,null,null,null,null,null,null,null},
                    {null,null,null,null,null,null,null,null},
                    {null,null,null,null,null,null,null,null},
                    {null,null,null,null,null,null,null,null},
                    {null,null,null,null,null,null,null,null},
                    {null,null,null,null,null,null,null,null},
                    {null,null,null,null,null,null,null,null}
            };
    }

    public static void initializeVariables() {
        downPos = null;
        lastPos = null;
        currentPiece = null;
        isWhiteTurn = true;
        clearPieces();
        Display.initializeVariables();
    }


}


