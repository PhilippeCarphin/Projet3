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

    private Point downPos;
    private Point lastPos;
    public Piece currentPiece;
    public boolean isWhiteTurn = true;
    public boolean isStarted = false;
    public ActivityGame activityGame;
    public Display display;

    public Point lastMoveOrigin, lastMoveDestination;

    public Piece[][] pieces;

    public Game(ActivityGame activityGame) {
        downPos = null;
        lastPos = null;
        lastMoveOrigin = null;
        lastMoveDestination = null;
        currentPiece = null;
        this.activityGame = activityGame;
        clearPieces();
        display = new Display(this);

    }

    //sets the piece starting positions
    private void setPieces(LinkedList<Piece> newPieces) {
        clearPieces();
        for (Piece newPiece : newPieces) {
            pieces[newPiece.p_.x][newPiece.p_.y] = newPiece;
        }
    }

    //gets the piece at a certain position
    private Piece getPieceOnCell(Point po) {
        return pieces[po.x][po.y];
    }

    //handler for when the user puts his finger on the chessboard
    public void handleFingerDown(int xPix, int yPix) throws Exception {
        downPos = display.getboardCoordinates(activityGame.board, xPix, yPix);
        Log.d("down", downPos.x+" "+downPos.y);
        if (downPos != null) {
            currentPiece = getPieceOnCell(downPos);

        }
        if (currentPiece != null) {
            if (currentPiece.isWhite_ != isWhiteTurn) {
                currentPiece = null;
                downPos = null;
            }
            else {
                pieces[downPos.x][downPos.y] = null;
                Log.d("Selected piece", currentPiece.type_);
                display.drawMotionlessPieces();
                display.drawMovingPiece();
            }
        }
    }

    //handler for when the user removes his finger from the chessboard
    public void handleFingerUp() {
        //Log.d("up", lastPos.x+" "+lastPos.y);
        if (currentPiece != null && !downPos.equals(lastPos)) {
            display.blockBoard(activityGame.board);
            HttpRunner.runPostMove(null, activityGame, isWhiteTurn, downPos.x, downPos.y, lastPos.x, lastPos.y,null);

        }
        else {

            handleMoveNotOk();
        }

    }

    //handler for when the user already has his finger on the screen and uses it
    public void handleMove(int xPix, int yPix) throws Exception {
        //lastPos = getboardCoordinates(xPix, yPix);

        if (downPos == null || currentPiece == null ) {
            return;
        }
        Point p = display.getboardCoordinates(activityGame.board, xPix, yPix);
        if (!p.equals(lastPos)) {
            lastPos = p;
            currentPiece.p_ = lastPos;
            display.drawMovingPiece();
        }
        //drawBoard();
    }

    //handles when the http returns that the asked move is ok
    public void handleMoveOk(String pieceEleminated, String promotion, String state) throws JSONException {
        if (currentPiece != null) {
            if (!pieceEleminated.equals("xx")) {
                Point p = Utilities.getGridCoordinates(pieceEleminated);
                pieces[p.x][p.y] = null;
            }
            pieces[currentPiece.p_.x][currentPiece.p_.y] = currentPiece;
            //isWhiteTurn = !isWhiteTurn;
            lastMoveOrigin = downPos;
            lastMoveDestination = currentPiece.p_;
        }
        HttpRunner.runGetStatusSummary(null, activityGame, null);
    }


    public void handleMoveNotOk() {
        if (currentPiece != null) {
            currentPiece.p_ = downPos;
            pieces[downPos.x][downPos.y] = currentPiece;
        }
        finishMove();
    }

    public void finishMove() {
        downPos = null;
        lastPos = null;
        currentPiece = null;
        display.drawFullBoard();
        display.drawMovingPiece();
        display.unBlockBoard(activityGame.board);
    }

    public void recoverFromError() {
        handleMoveNotOk();
    }

    public void setConfig(LinkedList<Piece> newPieces) {
        clearPieces();
        for (Piece p : newPieces) {
            pieces[p.p_.x][p.p_.y] = p;
        }
    }

    private void clearPieces() {
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





}


