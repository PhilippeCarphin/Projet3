package com.vintage.vintagechess;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.ImageView;

import java.util.LinkedList;

/**
 * Created by User on 2016-10-18.
 */

public class Game{

    public static ImageView chessBoard;
    public static int width, height;

    LinkedList<Piece> pieces;

    public void handleMove(int posX1, int posY1, int posX2, int posY2) {

    }

    private void initializePieces() {
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
                    name = "queen";
                }
                else {
                    name = "king";
                }
                Piece newPiece = new Piece(name, x, y, isWhite);
            }
        }
    }


    /*public static void drawBoard() {


        //Utilities.messageBox(Integer.valueOf(Integer.valueOf(width).toString(), "", activityGame);

        Bitmap tempBitmap = Bitmap.createBitmap(width, width, Bitmap.Config.RGB_565);//chessboard.getWidth(), chessboard.getHeight(), Bitmap.Config.RGB_565);
        Canvas tempCanvas = new Canvas(tempBitmap);
        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        paint.setColor(Color.BLUE);
        tempCanvas.drawRect(0, 0, 40, 40, paint);
        chessBoard.setImageDrawable(new BitmapDrawable(chessBoard.getResources(), tempBitmap));

    }*/

    private class Piece {

        int x_, y_;
        String resource_;
        public Piece(String type, int x, int y, boolean isWhite) {
            x_ = x;
            y_ = y;
            resource_ = "drawable/pieces/" + (isWhite ? "White" : "Black") + type +"1";  // where myresource (without the extension) is the file

        }
    }

}
