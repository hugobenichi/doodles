import java.util.*;
import java.io.*;


/*
 *  Cracking the coding interview, chap4, trees and graphs
 */
public class BTree<E> {

    public static final PrintStream stdout = System.out;

    public BTree<E> parent, left, right;
    public E value;

    public BTree(E value, BTree<E> parent) {
        this.value = value;
        this.parent = parent;
    }

    public BTree(E value) {
        this.value = value;
        this.parent = null;
    }

    public BTree<E> add_l(E value){
        this.left = new BTree<E>(value, this);
        return this.left;
    }

    public BTree<E> add_r(E value){
        this.right = new BTree<E>(value, this);
        return this.right;
    }

    /*
     *  4.1 check if a binary tree is balanced or not
     *  height difference between subtrees = 1
     */
    public boolean is_balanced(){
        return false;
    }

    /*
     *  4.3 transform an array of <E> into a binary search tree
     */
    public static <E> BTree<E> binary_search_tree(E[] array[]) {
        return null;
    }

    /*
     *  4.4 list of all the nodes at each depth
     */
    public ArrayList<E> hierarchy_list(){
        return null;
    }

    /*
     *  4.5 check if a binary tree is a binary search tree
     */
    public boolean is_binary_search_tree(){
        return false;
    }

    /*
     *  4.6 return next in-order node
     */
    public BTree<E> next_node(){
        return this;
    }

    /*
     *  4.7 return first common ancestor of this and other (use parents only)
     */
    public BTree<E> first_ancestor_with_parents(BTree<E> other){
        return this;
    }

    /*
     *  4.7 return first common ancestors of A and B in tree C (no parents)
     */
    public static <E> BTree<E> first_ancestor(BTree<E> a, BTree<E> b, BTree<E> c) {
        return c;
    }

    /*
     *  4.8 check if tree A is a subtree of this (this is very big).
     */
    public boolean is_subtree(BTree<E> a) {
        return false;
    }

    /*
     *  4.9 print all paths where the values of node sum to a given value
     */
    public List<List<E>> all_path_with_sum(int total){
        return null;
    }

    public static void main(String argv[]) {


        System.exit(0);
    }

}
