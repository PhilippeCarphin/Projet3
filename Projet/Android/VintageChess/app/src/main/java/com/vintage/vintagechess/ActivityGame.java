package com.vintage.vintagechess;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.PorterDuff;
import android.graphics.drawable.BitmapDrawable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.util.Pair;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;

import java.util.ArrayList;
import java.util.LinkedList;

public class ActivityGame extends AppCompatActivity implements View.OnTouchListener , View.OnClickListener{

    public static int X, Y;
    private final int INVALID_INDEX = -1;
    private ArrayList<Piece> pieces = new ArrayList<Piece>();
    private ArrayList<Piece> activePiece = new ArrayList<Piece>();
    private ArrayList<Point> mActiveDragPoints;
    public static int largeur = 152;
    public static int midLargeur = 152/2;

    private static int a = 0;
    ImageView chessBoard;
    ImageView display;
    ImageView redStyle1;
    ImageView redStyle2;
    ImageView bwStyle1;
    ImageView bwStyle2;
    ImageView blueStyle1;
    ImageView blueStyle2;
    ImageView woodStyle1;
    ImageView woodStyle2;
    int startX;
    int startY;

    Bitmap testPiece;
    ImageView testPiece2;
    int testPieceX = 1342;
    int testPieceY = 127;
    Canvas tempCanvas;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);

        chessBoard = (ImageView) findViewById(R.id.imageView);
        chessBoard.setImageResource(android.R.color.transparent);

        CreatePieces("white", "1");

        Game.chessBoard = chessBoard;
        chessBoard.setOnTouchListener(this);

        redStyle1 = (ImageView) findViewById(R.id.imageView1);
        redStyle2 =(ImageView) findViewById(R.id.imageView2);
        bwStyle1 = (ImageView) findViewById(R.id.imageView3);
        bwStyle2 =(ImageView) findViewById(R.id.imageView4);
        blueStyle1 = (ImageView) findViewById(R.id.imageView5);
        blueStyle2 =(ImageView) findViewById(R.id.imageView6);
        woodStyle1 =(ImageView) findViewById(R.id.imageView7);
        woodStyle2 =(ImageView) findViewById(R.id.imageView8);

        redStyle1.setOnClickListener(this);
        redStyle2.setOnClickListener(this);
        bwStyle1.setOnClickListener(this);
        bwStyle2.setOnClickListener(this);
        blueStyle1.setOnClickListener(this);
        blueStyle2.setOnClickListener(this);
        woodStyle1.setOnClickListener(this);
        woodStyle2.setOnClickListener(this);

        undoColorFilter();

    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        Game.width = chessBoard.getWidth();
        Game.height = chessBoard.getHeight();
        drawBoard();
    }

    /**********************************/
    public void CreatePieces(String color, String style){
        Piece pawn1 = new Piece(drawable(color, "pawn", style) ,1, style, color, "a7" );
        Piece pawn2 = new Piece(drawable(color, "pawn", style) ,2, style, color, "b7" );
        Piece pawn3 = new Piece(drawable(color, "pawn", style) ,3, style, color, "c7" );
        Piece pawn4 = new Piece(drawable(color, "pawn", style) ,4, style, color, "d7" );
        Piece pawn5 = new Piece(drawable(color, "pawn", style) ,5, style, color, "e7" );
        Piece pawn6 = new Piece(drawable(color, "pawn", style) ,6, style, color, "f7" );
        Piece pawn7 = new Piece(drawable(color, "pawn", style) ,7, style, color, "g7" );
        Piece pawn8 = new Piece(drawable(color, "pawn", style) ,8, style, color, "h7" );
        Piece rook1 = new Piece(drawable(color, "rook", style) ,9, style, color, "a8" );
        Piece knight1 = new Piece(drawable(color, "knight", style) ,10, style, color, "b8" );
        Piece bishop1 = new Piece(drawable(color, "bishop", style) ,11, style, color, "c8" );
        Piece queen = new Piece(drawable(color, "queen", style) ,12, style, color, "d8" );
        Piece king = new Piece(drawable(color, "king", style) ,13, style, color, "e8" );
        Piece bishop2 = new Piece(drawable(color, "bishop", style) ,14, style, color, "f8" );
        Piece knight2 = new Piece(drawable(color, "knight", style) ,15, style, color, "g8" );
        Piece rook2 = new Piece(drawable(color, "rook", style) ,16, style, color, "h8" );

        pieces.add(pawn1);
        pieces.add(pawn2);
        pieces.add(pawn3);
        pieces.add(pawn4);
        pieces.add(pawn5);
        pieces.add(pawn6);
        pieces.add(pawn7);
        pieces.add(pawn8);
        pieces.add(rook1);
        pieces.add(rook2);
        pieces.add(knight1);
        pieces.add(knight2);
        pieces.add(bishop1);
        pieces.add(bishop2);
        pieces.add(queen);
        pieces.add(king);

        Log.d("size", String.valueOf(pieces.size()));

    }

    /**********************************/
    public int drawable(String color, String PieceNeme, String style){
        String separator = "_";

        String nameOfResource = color+separator+PieceNeme+separator+style;

        Log.d("nameOfResource", nameOfResource);
        int resourceId = this.getResources().getIdentifier(nameOfResource, "drawable", this.getPackageName());
        Log.d("resourceId", String.valueOf(resourceId));
        return resourceId;
    }

    /**********************************/
    public void drawBoard() {

        Bitmap imagenAndroid = BitmapFactory.decodeResource(getResources(),R.drawable.bw_chess_board);
        imagenAndroid = Bitmap.createBitmap(imagenAndroid,0,0,2999,2999);
        tempCanvas = new Canvas(imagenAndroid);

        for(int i = 0; i<pieces.size(); i++ )
        {
            Bitmap imageBitmap = getBitmapImage(pieces.get(i).getId_());
            int xPos = pieces.get(i).getxPosition();
            int yPos = pieces.get(i).getyPosition();
            tempCanvas.drawBitmap(imageBitmap,xPos,yPos,null);  // largeure pas bonne
        }

        chessBoard.setImageDrawable(new BitmapDrawable(chessBoard.getResources(), imagenAndroid));
        chessBoard.setOnTouchListener(this);
    }

    /**********************************/
    public Bitmap getBitmapImage (int id) {
        Bitmap bmp = BitmapFactory.decodeResource( getResources(), id );
        Bitmap img = Bitmap.createScaledBitmap( bmp, 152*2, 152*2, true );
        bmp.recycle();

        Log.d("Id", String.valueOf(id));
        return img;
    }


    /**********************************/
    private void lookForIntersection(Point touchDown)
    {
        final int index = getIntersectionPieceIndex(touchDown);

        if( index != INVALID_INDEX )
        {
            final Piece piece = pieces.get(index);
            Log.d("pieces.indexOf(piece)", String.valueOf(pieces.indexOf(piece)));
            if( pieces.indexOf(piece) == INVALID_INDEX )
            {
                mActiveDragPoints.add(touchDown);
                activePiece.add(pieces.get(index));
            }
        }

    }


    private int getIntersectionPieceIndex(final Point point)
    {
        int index = INVALID_INDEX;
        for(Piece piece : pieces)
        {
            if( true/*(piece.getxPosition()-midLargeur<point.x) && (piece.getxPosition()+midLargeur>point.x)  && (piece.getyPosition() - midLargeur < point.y) && (point.y< piece.getyPosition()+midLargeur)*/ )
            {
                index = pieces.indexOf(piece);
                break;
            }
        }
        return index;
    }


    private void movePiece(Point currentPoint, Point prevPoint, final Piece piece)
    {
        int xMoved = currentPoint.x- prevPoint.x;
        int yMoved = currentPoint.y - prevPoint.y;
        piece.setPosition(xMoved, yMoved);
        mActiveDragPoints.set(mActiveDragPoints.indexOf(prevPoint), currentPoint);
    }


    @Override
    public boolean onTouch(View v, MotionEvent event) {

        switch (event.getAction())
        {
            case MotionEvent.ACTION_DOWN:
                Point touchDown = new Point((int)event.getX(), (int)event.getY());
                lookForIntersection(touchDown);
                break;
            case MotionEvent.ACTION_UP:

                break;
            case MotionEvent.ACTION_MOVE:
                int count = 0;
                for(Piece piece : activePiece)
                {
                    Point curretPoint = new Point((int)event.getX(count), (int)event.getY(count));
                    movePiece(curretPoint, mActiveDragPoints.get(count), piece);
                    count++;
                }
                break;
        }
        chessBoard.invalidate();
        drawBoard();
        return true;
    }

    /*Il reste encore du travail la dessus */
    @Override
    public void onClick(View v) {

        switch (v.getId())
        {
            case R.id.imageView1:
                chessBoard.setImageResource(R.drawable.red_chess_board);
                undoColorFilter();
                redStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView2:
                chessBoard.setImageResource(R.drawable.red_chess_board);
                undoColorFilter();
                redStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
               v.invalidate();
                break;
            case R.id.imageView3:
                chessBoard.setImageResource(R.drawable.bw_chess_board);
                undoColorFilter();
                bwStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView4:
                chessBoard.setImageResource(R.drawable.bw_chess_board);
                undoColorFilter();
                bwStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView5:
                chessBoard.setImageResource(R.drawable.blue_chess_board);
                undoColorFilter();
                blueStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView6:
                chessBoard.setImageResource(R.drawable.blue_chess_board);
                undoColorFilter();
                blueStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView7:
                chessBoard.setImageResource(R.drawable.wood_chess_board);
                undoColorFilter();
                woodStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView8:
                chessBoard.setImageResource(R.drawable.wood_chess_board);
                undoColorFilter();
                woodStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
        }

    }

    public void undoColorFilter(){
        redStyle1.getDrawable().setColorFilter(null);
        redStyle2.getDrawable().setColorFilter(null);
        bwStyle1.getDrawable().setColorFilter(null);
        bwStyle2.getDrawable().setColorFilter(null);
        blueStyle1.getDrawable().setColorFilter(null);
        blueStyle2.getDrawable().setColorFilter(null);
        woodStyle1.getDrawable().setColorFilter(null);
        woodStyle2.getDrawable().setColorFilter(null);
    }



}









