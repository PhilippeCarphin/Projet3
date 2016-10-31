package com.vintage.vintagechess;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import android.graphics.Canvas;
import android.graphics.Point;
import android.graphics.PorterDuff;
import android.graphics.drawable.BitmapDrawable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;

import org.json.JSONException;

import java.util.ArrayList;

//
//inspiré de http://stackoverflow.com/questions/20881790/how-to-move-multiple-bitmaps-in-single-canvas-android
//
public class ActivityGame extends AppCompatActivity implements View.OnTouchListener , View.OnClickListener{

    private final int INVALID_INDEX = -1;
    private ArrayList<Piece> pieces = new ArrayList<Piece>();
    private ArrayList<Piece> activePiece = new ArrayList<Piece>();
    private ArrayList<Point> mActiveDragPoints= new ArrayList<Point>();

    int boardId_;

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

    private String[] whitePosition = new String[]{"a7", "b7","c7", "d7","e7", "f7","g7", "h7","a8", "b8","c8", "d8","e8", "f8","g8", "h8" };
    private String[] blackPosition = new String[]{"a2", "b2","c2", "d2","e2", "f2","g2", "h2","a1", "b1","c1", "d1","e1", "f1","g1", "h1" };


    public int getBoardId()
    {
        return boardId_;

    }

    public void setBoardId(int id)
    {
        boardId_ = id;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);

        chessBoard = (ImageView) findViewById(R.id.imageView);
        chessBoard.setImageResource(android.R.color.transparent);

        Game.chessBoard = chessBoard;

        boardId_ = drawableChessBoard("bw_chess");

        chessBoard.setOnTouchListener(this);

        CreatePieces("white", "1");
        CreatePieces("black", "1");

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
        String[] position;
        if(color == "white")
        {
            position = whitePosition;
        }else
        {
            position = blackPosition;
        }
        Piece pawn1 = new Piece(drawablePieces(color, "pawn", style) ,1, style, color,"pawn", position[0] );
        Piece pawn2 = new Piece(drawablePieces(color, "pawn", style) ,2, style, color,"pawn", position[1]);
        Piece pawn3 = new Piece(drawablePieces(color, "pawn", style) ,3, style, color,"pawn", position[2] );
        Piece pawn4 = new Piece(drawablePieces(color, "pawn", style) ,4, style, color,"pawn", position[3] );
        Piece pawn5 = new Piece(drawablePieces(color, "pawn", style) ,5, style, color,"pawn", position[4] );
        Piece pawn6 = new Piece(drawablePieces(color, "pawn", style) ,6, style, color,"pawn", position[5]);
        Piece pawn7 = new Piece(drawablePieces(color, "pawn", style) ,7, style, color,"pawn", position[6] );
        Piece pawn8 = new Piece(drawablePieces(color, "pawn", style) ,8, style, color,"pawn", position[7] );
        Piece rook1 = new Piece(drawablePieces(color, "rook", style) ,9, style, color,"rook", position[8] );
        Piece knight1 = new Piece(drawablePieces(color, "knight", style) ,10, style, color,"knight", position[9] );
        Piece bishop1 = new Piece(drawablePieces(color, "bishop", style) ,11, style, color,"bishop", position[10] );
        Piece queen = new Piece(drawablePieces(color, "queen", style) ,12, style, color,"queen", position[11] );
        Piece king = new Piece(drawablePieces(color, "king", style) ,13, style, color,"king", position[12] );
        Piece bishop2 = new Piece(drawablePieces(color, "bishop", style) ,14, style, color,"bishop", position[13] );
        Piece knight2 = new Piece(drawablePieces(color, "knight", style) ,15, style, color,"knight", position[14] );
        Piece rook2 = new Piece(drawablePieces(color, "rook", style) ,16, style, color,"rook", position[15] );

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


    }

    /**********************************/
    public int drawablePieces(String color, String PieceNeme, String style){
        String separator = "_";

        String nameOfResource = color+separator+PieceNeme+separator+style;

        //Log.d("nameOfResource", nameOfResource);
        int resourceId = this.getResources().getIdentifier(nameOfResource, "drawable", this.getPackageName());
        //Log.d("resourceId", String.valueOf(resourceId));
        return resourceId;
    }

    public int drawableChessBoard(String style){
        String separator = "_";
        String nameOfResource = style+separator+"board";

        //Log.d("nameOfResource", nameOfResource);
        int resourceId = this.getResources().getIdentifier(nameOfResource, "drawable", this.getPackageName());
        //Log.d("resourceId", String.valueOf(resourceId));
        return resourceId;
    }

    /**********************************/
    public void drawBoard() {


        Bitmap imagenAndroid = BitmapFactory.decodeResource(getResources(),boardId_);
        //imagenAndroid = Bitmap.createBitmap(imagenAndroid,0,0,2999,2999);
        imagenAndroid = Bitmap.createScaledBitmap( imagenAndroid, Game.width , Game.width , true );
        tempCanvas = new Canvas(imagenAndroid);

        for(int i = 0; i<pieces.size(); i++ )
        {
            Bitmap imageBitmap = getBitmapImage(pieces.get(i).getId_());
            int xPos =  pieces.get(i).getxPosition();
            int yPos =  pieces.get(i).getyPosition();
            //Log.d("xPosyPos", String.valueOf(xPos) +" "+ String.valueOf(yPos));
            tempCanvas.drawBitmap(imageBitmap,xPos,yPos,null);  // largeure pas bonne
        }

        chessBoard.setImageDrawable(new BitmapDrawable(chessBoard.getResources(), imagenAndroid));
        chessBoard.setOnTouchListener(this);
    }

    /**********************************/
    public Bitmap getBitmapImage (int id) {
        Bitmap bmp = BitmapFactory.decodeResource( getResources(), id );
        Bitmap img = Bitmap.createScaledBitmap( bmp, 166, 166, true );
        bmp.recycle();

        //Log.d("Id", String.valueOf(id));
        return img;
    }


    /**********************************/
    private void lookForIntersection(Point touchDown)
    {
        final int index = getIntersectionPieceIndex(touchDown);

        if( index != INVALID_INDEX )
        {
            final Piece piece = pieces.get(index);
            if( activePiece.indexOf(piece) == INVALID_INDEX )
            {
                mActiveDragPoints.add(touchDown);
                activePiece.add(pieces.get(index));
               // Log.d("activePiece ", String.valueOf(activePiece.size()));
            }
        }

    }


    private int getIntersectionPieceIndex(final Point point)
    {
        String posSymb = positionToSymbol(point.x, point.y);
        int index = INVALID_INDEX;
        for(int i = 0; i< pieces.size(); i++ )
        {
            String symboleDeLaPiece = pieces.get(i).getSymbole();

           // Log.d("symboleSelectionner ", posSymb +" symbole de la piece "+ symboleDeLaPiece);
            if( posSymb.equals(symboleDeLaPiece) )
            {
                //Log.d("pieces  name ", String.valueOf(pieces.get(i).getName()) +" COLOR: "+ String.valueOf(pieces.get(i).getColor()) +" STYLE: "+ String.valueOf(pieces.get(i).getStyle()));
                index = pieces.indexOf(pieces.get(i));
                break;
            }

        }

       // Log.d("index ", String.valueOf(index));

        return index;
    }


    public String positionToSymbol(int x, int y)
    {
        int valXX = ((x-130)/151);
        String valX = Character.toString ((char) (valXX+97));
        //char valX =  (char) valXX;  /// affiche un carre faut arranger ca
        int valY = 8 - ((y-62)/151);

        String symbole = valX+String.valueOf(valY);
        //Log.d("symbole ", symbole );

        return symbole;

    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {

        final int action  = event.getActionMasked();
        final int pointer = event.getActionIndex();

        switch (action) {
            case MotionEvent.ACTION_DOWN :
                Point touchDown = new Point((int)event.getX(), (int)event.getY());
                lookForIntersection(touchDown);
                break;
            case MotionEvent.ACTION_UP :
            case MotionEvent.ACTION_CANCEL :
                mActiveDragPoints.removeAll(mActiveDragPoints);
                activePiece.removeAll(activePiece);
                break;
            case MotionEvent.ACTION_MOVE :
                //Log.d("PIECE SELECTionner", String.valueOf(activePiece.get(0).getSymbole()));
                String Symbole = positionToSymbol((int)event.getX(), (int)event.getY());
                //activePiece.get(0).SymboleToXY((int)event.getX(), (int)event.getY())
                activePiece.get(0).setSymbole(Symbole);
                break;
            case MotionEvent.ACTION_POINTER_UP :
                int index = getIntersectionPieceIndex(new Point((int)event.getX(pointer), (int)event.getY(pointer)));
                if( index != INVALID_INDEX )
                {
                    Piece piece = activePiece.get(index);
                    mActiveDragPoints.remove(activePiece.indexOf(piece));
                    activePiece.remove(piece);
                }
                break;


            default:
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
                changePiecesAndBoard("1", "red_chess");
                undoColorFilter();
                redStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);

                v.invalidate();
                break;
            case R.id.imageView2:
                changePiecesAndBoard("2", "red_chess");
                undoColorFilter();
                redStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
               v.invalidate();
                break;
            case R.id.imageView3:
                changePiecesAndBoard("1", "bw_chess");
                undoColorFilter();
                bwStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView4:
                changePiecesAndBoard("2", "bw_chess");
                undoColorFilter();
                bwStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView5:
                changePiecesAndBoard("1", "blue_chess");
                undoColorFilter();
                blueStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView6:
                changePiecesAndBoard("2", "blue_chess");
                undoColorFilter();
                blueStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView7:
                changePiecesAndBoard("1", "wood_chess");
                undoColorFilter();
                woodStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView8:
                changePiecesAndBoard("2", "wood_chess");
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

    public void changePiecesAndBoard(String piecesStyle, String boardStyle){

        int newIdBoard = drawableChessBoard(boardStyle);
        boardId_ = newIdBoard;
        chessBoard.setImageResource(boardId_);

        for(int i = 0; i<pieces.size(); i++)
        {
            pieces.get(i).setStyle(piecesStyle);
            int newIdPiece = drawablePieces(pieces.get(i).getColor(), pieces.get(i).getName(), piecesStyle);
            pieces.get(i).setId_(newIdPiece);
        }

        drawBoard();
    }

}









