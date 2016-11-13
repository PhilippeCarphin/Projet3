package com.vintage.vintagechess;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Point;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;
import android.widget.ImageView;

import java.util.ArrayList;
import java.util.LinkedList;

/**
 * Created by User on 2016-10-18.
 */

public class Game {

    public static String style;
    private static Point downPos;
    private static Point lastPos;
    public static Piece currentPiece;
    public static int leftSpace;
    public static String ip;
    private static boolean isWhiteTurn = true;


    public static Piece[][] pieces;


    //set the parameters by default
    public static void initializeGame() {

        Display.board_style = "red";
        style = "1";
        initializePieces();
        Display.setBoardImg();
    }

    //sets the piece starting positions
    private static void initializePieces() {
        pieces = Utilities.getEmptyGrid();
        int [] verticals = {0, 1, 6, 7};
        for  (int x = 0 ; x < 8 ; x++) {
            for (int y : verticals) {
                boolean isWhite = y < 4;

                String name;
                if (y % 7 != 0) { //pawns
                    name = "pawn";
                }
                else if (x % 7 == 0) {
                    name = "rook";
                }
                else if ((x - 1) % 5 == 0) {
                    name = "knight";
                }
                else if ((x - 2) % 3 == 0) {
                    name = "bishop";
                }
                else if (x == 4) {
                    name = "king";
                }
                else {
                    name = "queen";
                }
                Piece newPiece = new Piece(name, x, y, isWhite);
                pieces[newPiece.p_.x][newPiece.p_.y] = newPiece;
            }
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

        //HttpRunner.runGetStatusBoard();
        //drawBoard();
    }

    //handles when the http returns that the asked move is ok
    public static void handleMoveOk(String pieceEleminated, String promotion, String state) {
        if (currentPiece != null) {
            if (!pieceEleminated.equals("xx")) {
                Point p = Utilities.getGridCoordinates(pieceEleminated);
                pieces[p.x][p.y] = null;
            }
            pieces[currentPiece.p_.x][currentPiece.p_.y] = currentPiece;
            isWhiteTurn = !isWhiteTurn;
        }
        finishMove();
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
        Display.drawMotionlessPieces();
        Display.drawMovingPiece();
        Display.unBlockBoard();
    }

    public static void recoverFromError() {
        handleMoveNotOk();
    }








}


