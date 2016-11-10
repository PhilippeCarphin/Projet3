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


    public static ImageView board;
    public static ImageView motionlessPieces;
    public static ImageView movingPiece;
    public static Context activityGame;
    public static String board_style;
    public static String style;
    public static double offset = 0.05133;
    private static Point downPos;
    private static Point lastPos;
    private static Piece currentPiece;
    private static Bitmap boardImg;
    public static int leftSpace;
    public static String ip;

    private static ArrayList<Piece> pieces = new ArrayList<>();
    private static LinkedList<Piece> deletedPieces = new LinkedList<Piece>();


    public static void initializeGame() {

        board_style = "red";
        style = "1";
        initializePieces();
        setBoardImg();
    }

    private static void initializePieces() {
        pieces.clear();
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
                pieces.add(newPiece);
            }
        }
    }

    public static void drawFullBoard() {
        drawBoard();
        drawMotionlessPieces();
    }

    public static void drawBoard() {

        Bitmap imageAndroid = Bitmap.createBitmap(board.getHeight(), board.getHeight(), Bitmap.Config.ARGB_8888);
        Canvas tempCanvas = new Canvas(imageAndroid);
        tempCanvas.drawBitmap(boardImg,0,0,null);

        board.setImageDrawable(new BitmapDrawable(board.getResources(), imageAndroid));

    }

    public static void drawMotionlessPieces() {
        //int id = activityGame.getResources().getIdentifier(board_style + "_chess_board", "drawable", activityGame.getPackageName());
        Bitmap imageAndroid = Bitmap.createBitmap(board.getHeight(), board.getHeight(), Bitmap.Config.ARGB_8888);
        //boardImg.copy(boardImg.getConfig(), true);
        Canvas tempCanvas = new Canvas(imageAndroid);
        for(Piece p : pieces) {
            Bitmap imageBitmap = p.getBitmap();
            Point position = p.p_;
            int xPos =  getPosition(position.x);
            int yPos =  getPosition(position.y);
            tempCanvas.drawBitmap(imageBitmap,xPos,yPos,null);
        }

        motionlessPieces.setImageDrawable(new BitmapDrawable(board.getResources(), imageAndroid));
        //board.setOnTouchListener(activityGame);
    }

    public static void drawMovingPiece() {
        Bitmap imageAndroid = Bitmap.createBitmap(board.getHeight(), board.getHeight(), Bitmap.Config.ARGB_8888);

        if (currentPiece != null) {
            Canvas tempCanvas = new Canvas(imageAndroid);
            Bitmap imageBitmap = currentPiece.getBitmap();
            Point position = currentPiece.p_;
            int xPos =  getPosition(position.x);
            int yPos =  getPosition(position.y);
            tempCanvas.drawBitmap(imageBitmap,xPos,yPos,null);
            tempCanvas.drawBitmap(imageBitmap,xPos,yPos,null);
        }

        movingPiece.setImageDrawable(new BitmapDrawable(board.getResources(), imageAndroid));
    }

    public static int getSquareWidth() {
        return (int)((1-2*offset)*board.getHeight()/8.0);
    }

    public static int getBoardOffset() {
        return (int)(board.getHeight() * offset);
    }

    public static int getPosition(int d) {
        return getBoardOffset() + d*getSquareWidth();
    }

    private static Piece getPieceOnCell(Point po) {
        for (Piece p : pieces) {
            if (p.p_.equals(po)) return p;
        }
        return null;
    }

    public static void handleFingerDown(int xPix, int yPix) {
        downPos = getboardCoordinates(xPix, yPix);
        if (downPos != null) {
            currentPiece = getPieceOnCell(downPos);
        }
        if (currentPiece != null) {
            pieces.remove(currentPiece);
            drawMotionlessPieces();
        }
    }

    private static Point getboardCoordinates(int xPix, int yPix) {
        int x, y, xView, yView;
        int min, max;

        xView = xPix - (board.getWidth()-board.getHeight())/2;
        yView = yPix;
        min = getBoardOffset();
        max = board.getHeight() - getBoardOffset();
        if (xView < min || xView > max || yView < min || yView > max) {
            Log.d("", "out of bounds");
            return null;
        }
        float insideWidth = board.getHeight()-getBoardOffset()*2;
        x = (int)(8*(((float)(xView - getBoardOffset()))/insideWidth));
        y = (int)(8*(((float)(yView - getBoardOffset()))/insideWidth));
        return new Point(x, y);
    }

    public static void handleFingerUp() {
        if (currentPiece != null) {
            pieces.add(currentPiece);
        }
        downPos = null;
        lastPos = null;
        currentPiece = null;
        drawMotionlessPieces();
        drawMovingPiece();
        //new RequestTask().execute("http://www.google.com");
    }

    public static void handleMove(int xPix, int yPix) {
        if (downPos == null || currentPiece == null ) {
            handleFingerUp();
            return;
        }
        Point p = getboardCoordinates(xPix, yPix);
        if (lastPos == null) {
            lastPos = p;
            currentPiece.p_.set(lastPos.x, lastPos.y);
            drawMovingPiece();
        }
        else if (!p.equals(lastPos.x, lastPos.y)) {
            //Log.d("changed position", "");
            currentPiece.p_.set(p.x, p.y);
            drawMovingPiece();
        }
        //drawBoard();
    }

    public static void setBoardImg() {
        int id = activityGame.getResources().getIdentifier(board_style + "_chess_board", "drawable", activityGame.getPackageName());
        boardImg = BitmapFactory.decodeResource(activityGame.getResources(), id);
        //imagenAndroid = Bitmap.createBitmap(imagenAndroid,0,0,2999,2999);
        boardImg = Bitmap.createScaledBitmap( boardImg, board.getHeight() , board.getHeight() , true );
    }

    public static void deletePiece(String boardPosition)
    {

        //Bug de duplication

        int x =  Integer.valueOf(boardPosition.charAt(0)) - 97; // 97 cest le a dans la table ascii
        int y = 8 - Integer.valueOf(boardPosition.charAt(1)) + 48; // 48 cest la 1 dans la table ascii

        Point point = new Point(x,y);

        for(int i = 0; i < pieces.size(); i++)  // Par tres optimal pour chercher la piece O(n)
        {
            if(pieces.get(i).p_.equals(point))
            {
                deletedPieces.add(pieces.get(i));
                pieces.remove(pieces.get(i));
            }
        }

        for(Piece p : deletedPieces) {
            Log.d(" DeletedPieces ", p.type_);
        }
    }


}


