package com.vintage.vintagechess;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ImageView;

public class ActivityGame extends AppCompatActivity {

    ImageView chessBoard;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);

        chessBoard = (ImageView) findViewById(R.id.imageView) ;
        chessBoard.setImageResource(android.R.color.transparent);
        String uri = "drawable/pieces/1BlackKnight";

//        // int imageResource = R.drawable.icon;
//        int imageResource = getResources().getIdentifier(uri, null, getPackageName());
//        chessBoard.setImageResource(imageResource);
//        //Bitmap bitmap =
//        Game.context = this.getApplicationContext();
    }
}
