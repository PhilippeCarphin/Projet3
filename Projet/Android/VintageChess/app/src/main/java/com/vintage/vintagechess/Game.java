package com.vintage.vintagechess;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.provider.MediaStore;
import android.widget.ImageView;

/**
 * Created by User on 2016-10-18.
 */

public class Game {

    public void handleMove(int posX, int posY) {

    }

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
