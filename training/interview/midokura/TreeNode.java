import java.util.*;


public class TreeNode {

    TreeNode parent;   // I assumed that the top node's parent is null

    /** wrapper method to access a node's parent */
    public TreeNode parent() { return parent; }

    /**
     *  We walk back the tree in parallel starting from this and other, one step
     *  at a time. All visited node are indexed in a set. If the set already
     *  contains a node, we have found a common ancestor. We can use a hash
     *  which gives average O(1) insertion and O(1) test for presence.
     *
     *  We use a HashSet<TreeNode> to index the nodes, which tests presence with
     *  equals. If equals is not overloaded it tests pointer equality with ==
     *  (default behavior of Object). It is fine except if the TreeNode class
     *  carries some kind of value and has the equals method overloaded, then we 
     *  could falsely identify different nodes carrying the same value as a common
     *  ancestor. In that case we can replace HashSet<TreeNode> by
     *  IdentityHashMap and stores dummy null values.
     *
     *  If both TreeNodes are from different trees, I assumed the method should
     *  returns null (no common ancestor).
     *
     *  The method is not safe to use with degenerate cyclic trees. If given two
     *  nodes from two degenerate cyclic trees, the method does not terminate.
     *
     *  Runtime: The runtime will be proportional to the depth of the subtree
     *  which starts from the common ancestor down to the deepest of both nodes.
     *  I am not sure of the details of implementation of the hashset class but
     *  I believe it should use memory roughly proportional to the number of
     *  visited nodes.
     *
     *  Time to solve: About 5 mins to decide which algorithm I would implement.
     *  Then 10 mins to write it, and about 30 mins to check which Java class I
     *  should use and be sure about the == vs equals issue.
     */
    TreeNode findFirstCommonAncester(TreeNode other){
        if (other == null) return null;        // guard against null input

        HashSet<TreeNode> visited_nodes = new HashSet<TreeNode>();

        TreeNode x = this, y = other, parent = null;

        while x != null && y != null) {
            if (x != null){
                parent = x.parent();
                boolean was_new = visited_nodes.add(parent);
                if (!was_new) return parent;
                x = parent;
            }
            if (y != null){
                parent = y.parent();
                boolean was_new = visited_nodes.add(parent);
                if (!was_new) return parent;
                y = parent;
            }
        }

        /** This point is reached if both nodes are from different trees and
         *  we walked back both trees to their top. In that case we return null;
         */
        return null;

    }

}
