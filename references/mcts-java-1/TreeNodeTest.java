import java.util.LinkedList;
import java.util.List;

/**
 * Created by Simon M. Lucas
 * sml@essex.ac.uk
 * Date: 03-Dec-2010
 * Time: 21:51:31
 */
public class TreeNodeTest {
    public static void main(String[] args) {
        List<TreeNode> list = new LinkedList<TreeNode>();
        list.add(null);
        System.out.println("list: " + list);
        TreeNode tn = new TreeNode();
        ElapsedTimer t = new ElapsedTimer();
        int n = 1000;
        for (int i=0; i<n; i++) {
            tn.selectAction();
        }
        System.out.println(t);
        TreeView tv = new TreeView(tn);
        tv.showTree("After " + n + " play outs");
        System.out.println("Done");
    }
}