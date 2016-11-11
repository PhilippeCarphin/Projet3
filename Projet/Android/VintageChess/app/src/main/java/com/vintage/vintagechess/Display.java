package com.vintage.vintagechess;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Point;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;
import android.widget.ImageView;

/**
 * Created by User on 2016-11-11.
 */

public class Display {
    public static ImageView board;
    public static ImageView motionlessPieces;
    public static ImageView movingPiece;
    public static double offset = 0.05133;
    private static Bitmap boardImg;
    public static String board_style;


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

    public static int getSquareWidth() {
        return (int)((1-2*offset)*board.getHeight()/8.0);
    }

    public static int getBoardOffset() {
        return (int)(board.getHeight() * offset);
    }

    public static int getPixelPosition(int d) {
        return getBoardOffset() + d*getSquareWidth();
    }

    public static boolean isOutOfBounds(Point p) {
        return false;
    }

    public static void setBoardImg() {

        int id = Utilities.currentActivity.getResources().getIdentifier(board_style + "_chess_board", "drawable", Utilities.currentActivity.getPackageName());
        boardImg = BitmapFactory.decodeResource(Utilities.currentActivity.getResources(), id);
        //imagenAndroid = Bitmap.createBitmap(imagenAndroid,0,0,2999,2999);
        boardImg = Bitmap.createScaledBitmap( boardImg, board.getHeight() , board.getHeight() , true );
    }

    public static Point getboardCoordinates(int xPix, int yPix) {
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
        return new Point(x, 7-y);
    }
}
