package com.vintage.vintagechess;

/**
 * Created by PO on 2016-10-25.
 */

public class Pieces {

    public String Style_;
    public String color_;
    public Piece bishop_;
    public Piece king_;
    public Piece knight_;
    public Piece pawn_;
    public Piece queen_;
    public Piece rook_;

    public Pieces(String Style,String color)
    {
        ChooseStyleAndColorPiece(Style,color);
    }

    public void ChooseStyleAndColorPiece(String Style,String color)
    {
        int idWK = R.drawable.white_knight_1;
        knight_= new Piece(idWK, 1,"1", "white", 123, 123);
    }

    public Piece Getknight()
    {
        return knight_;
    }

}
