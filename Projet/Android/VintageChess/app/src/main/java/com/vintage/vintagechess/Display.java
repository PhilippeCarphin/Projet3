package com.vintage.vintagechess;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;
import android.util.TypedValue;
import android.widget.ImageView;

/**
 * Created by User on 2016-11-11.
 */

public class Display {
    public static ImageView board;
    public static ImageView motionlessPieces;
    public static ImageView movingPiece;
    public static double offset = 0.05133;
    //private static Bitmap boardImg;
    public static String board_style;
    public static int blackSquareColor; // = Color.rgb(0xEE, 0xEE, 0xD2);
    public static int whiteSquareColor; // = Color.rgb(0x76, 0x96, 0x56);
    public static int borderColor = Color.rgb(0x95, 0x5C, 0x3E);
    private static int numberColor = Color.WHITE;
    public static ActivityGame activityGame;

    //draws the board and its pieces
    public static void drawFullBoard() {
        drawBoard();
        drawMotionlessPieces();
    }

    //draws the chessboard image
    public static void drawBoard() {

        Bitmap imageAndroid = Bitmap.createBitmap(board.getHeight(), board.getHeight(), Bitmap.Config.ARGB_8888);
        Canvas tempCanvas = new Canvas(imageAndroid);
        //tempCanvas.drawBitmap(boardImg,0,0,null);
        //display the first square
        Paint backPaint = new Paint();
        backPaint.setColor(borderColor);
        tempCanvas.drawRect(0, 0, board.getHeight(), board.getHeight(), backPaint);

        //display the squares
        for (int i = 0 ; i < 8 ; i++) {
            for (int j = 0 ; j < 8 ; j++) {
                int left = getPixelPosition(i);
                int right = getPixelPosition(i+1);
                int top = getPixelPosition(j);
                int bottom = getPixelPosition(j+1);
                int c = (((i + j) % 2) == 0) ? blackSquareColor : whiteSquareColor;
                Paint squarePaint = new Paint();
                squarePaint.setColor(c);

                tempCanvas.drawRect((float)left, (float)top, (float)right, (float)bottom, squarePaint);
            }
        }

        //display the numbers
        //Paint numberPaint = new Paint();
        //numberPaint.setTextAlign(Paint.Align.CENTER);
        Paint numberPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG);
        numberPaint.setColor(numberColor);
        numberPaint.setTextSize((int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_SP, 12, Utilities.currentActivity.getResources().getDisplayMetrics()));
        numberPaint.setTextAlign(Paint.Align.CENTER);
        Paint.FontMetrics metric = numberPaint.getFontMetrics();
        int textHeight = (int) Math.ceil(metric.descent - metric.ascent);
        for (int i = 0 ; i < 8 ; i++) {
            String charToDraw = String.valueOf((char)(i+65));
            int x = getBoardOffset() + (int)(getSquareWidth() * (i + 0.5));
            int y = (int)(textHeight - metric.descent) + getBoardOffset()/2 + (int)((numberPaint.descent() + numberPaint.ascent()) / 2);
            tempCanvas.drawText(charToDraw, x, y, numberPaint);
            y = y + 8*getSquareWidth() + getBoardOffset();
            tempCanvas.drawText(charToDraw, x, y, numberPaint);
            charToDraw = "" + (8-i);
            x = getBoardOffset()/2;
            y = (int)(textHeight/2 - metric.descent) + getBoardOffset() + (int)(getSquareWidth() * (i+0.5));
            tempCanvas.drawText(charToDraw, x, y, numberPaint);
            x += getSquareWidth() * 8 + getBoardOffset();
            tempCanvas.drawText(charToDraw, x, y, numberPaint);

        }
        //tempCanvas.drawText("allo", 0, 0, numberPaint);

        board.setImageDrawable(new BitmapDrawable(board.getResources(), imageAndroid));

    }

    //draws the pieces that are not selected in the appropriate board
    public static void drawMotionlessPieces() {
        //int id = activityGame.getResources().getIdentifier(board_style + "_chess_board", "drawable", activityGame.getPackageName());
          Bitmap imageAndroid = Bitmap.createBitmap(board.getHeight(), board.getHeight(), Bitmap.Config.ARGB_8888);
        //boardImg.copy(boardImg.getConfig(), true);
        Canvas tempCanvas = new Canvas(imageAndroid);
        for(Piece[] array : Game.pieces) {
            for (Piece p : array)  {
                if (p != null) {
                    Bitmap imageBitmap = p.getBitmap();
                    Point position = p.p_;
                    int xPos = getPixelPosition(position.x);
                    int yPos = getPixelPosition(7 - position.y);
                    tempCanvas.drawBitmap(imageBitmap, xPos, yPos, null);
                }
            }
        }

        motionlessPieces.setImageDrawable(new BitmapDrawable(board.getResources(), imageAndroid));
        //board.setOnTouchListener(activityGame);
    }

    //draw the curent moving piece in the appropriate view
    public static void drawMovingPiece() {
        Bitmap imageAndroid = Bitmap.createBitmap(board.getHeight(), board.getHeight(), Bitmap.Config.ARGB_8888);

        if (Game.currentPiece != null) {
            Canvas tempCanvas = new Canvas(imageAndroid);
            Bitmap imageBitmap = Game.currentPiece.getBitmap();
            Point position = Game.currentPiece.p_;
            int xPos =  getPixelPosition(position.x);
            int yPos =  getPixelPosition(7-position.y);
            tempCanvas.drawBitmap(imageBitmap,xPos,yPos,null);
            tempCanvas.drawBitmap(imageBitmap,xPos,yPos,null);
        }

        movingPiece.setImageDrawable(new BitmapDrawable(board.getResources(), imageAndroid));
    }

    //get the size in pixels of a square in the board
    public static int getSquareWidth() {
        return (int)((1-2*offset)*board.getHeight()/8.0);
    }

    //get the size of the board border in pixels
    public static int getBoardOffset() {
        return (int)(board.getHeight() * offset);
    }

    //get the pixel position of the beginning of the square d
    public static int getPixelPosition(int d) {
        return getBoardOffset() + d*getSquareWidth();
    }

    public static boolean isOutOfBounds(Point p) {
        return false;
    }

    //update the chessboard image
    public static void setBoardImg() {

        int id = Utilities.currentActivity.getResources().getIdentifier(board_style + "_chess_board", "drawable", Utilities.currentActivity.getPackageName());
        //boardImg = BitmapFactory.decodeResource(Utilities.currentActivity.getResources(), id);
        //imagenAndroid = Bitmap.createBitmap(imagenAndroid,0,0,2999,2999);
        //boardImg = Bitmap.createScaledBitmap( boardImg, board.getHeight() , board.getHeight() , true );
    }


    //convertir les coordonnees de pixel en coordonnees dans la grille de l'echiquier
    public static Point getboardCoordinates(int xPix, int yPix) throws Exception {
        int x, y, xView, yView;
        int min, max;

        xView = xPix - (board.getWidth()-board.getHeight())/2;
        yView = yPix;
        min = getBoardOffset();
        max = board.getHeight() - getBoardOffset();
        if (xView < min || xView > max || yView < min || yView > max) {
            throw new Exception("Finger is out of bounds");
        }
        float insideWidth = board.getHeight()-getBoardOffset()*2;
        x = (int)(8*(((float)(xView - getBoardOffset()))/insideWidth));
        y = (int)(8*(((float)(yView - getBoardOffset()))/insideWidth));
        return new Point(x, 7-y);
    }


    //block the chessboard view
    public static void blockBoard() {
        board.setEnabled(false);
    }

    //unblock the chessboard view
    public static void unBlockBoard() {
        board.setEnabled(true);
    }


    public static void setMoveNumber(String n) {
        activityGame.moveNumber.setText(n);
    }

    public static void setWhoseTurn(String turn) {
        Game.isWhiteTurn = turn.equals("1");
        activityGame.turn.setText(Game.isWhiteTurn ? "White" : "Black");

    }

    public static void statusSummary(String turnReceived, String moveNbReceived, String lastMoveReceived, String stateReceived ){
        setMoveNumber(moveNbReceived);
        setWhoseTurn(turnReceived);
        //activityGame.state.setText(stateReceived);


    }

}
