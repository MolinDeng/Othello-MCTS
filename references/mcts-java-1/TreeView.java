import javax.swing.*;
import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.util.HashMap;

/**
 * Created by Simon M. Lucas
 * sml@essex.ac.uk
 * Date: 29-Nov-2010
 * Time: 22:23:45
 * <p/>
 * Simple TreeView for a MCTS Tree
 */
public class TreeView extends JComponent {
    TreeNode root;
    int nw = 30;
    int nh = 20;
    int inset = 20;
    int minWidth = 300;
    int heightPerLevel = 40;
    Color fg = Color.black;
    Color bg = Color.cyan;
    Color nodeBg = Color.white;
    Color highlighted = Color.red;
    // the highlighted set of nodes...
    HashMap<TreeNode, Color> high;

    public TreeView(TreeNode root) {
        this.root = root;
        high = new HashMap<TreeNode, Color>();
    }

    public void paintComponent(Graphics gg) {
        // Font font =
        // g.setFont();
        Graphics2D g = (Graphics2D) gg;
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        int y = inset;
        int x = getWidth() / 2;
        g.setColor(bg);
        g.fillRect(0, 0, getWidth(), getHeight());
        draw(g, root, x, y, (int) (1.1 * getWidth()) - inset * 0);
    }

    private void draw(Graphics2D g, TreeNode cur, int x, int y, int wFac) {
        // draw this one, then it's children

        int arity = cur.arity();
        for (int i = 0; i < arity; i++) {
            if (cur.children[i].nVisits > 0) {
                int xx = (int) ((i + 1.0) * wFac / (arity + 1) + (x - wFac / 2));
                int yy = y + heightPerLevel;
                g.setColor(fg);
                g.drawLine(x, y, xx, yy);
                draw(g, cur.children[i], xx, yy, wFac / arity);
            }
        }
        drawNode(g, cur, x, y);
    }

    private void drawNode(Graphics2D g, TreeNode node, int x, int y) {
        String s = (int) node.totValue + "/" + (int) node.nVisits;
        g.setColor(nodeBg);
        // if (high.contains(node)) g.setColor(highlighted);
        g.fillOval(x - nw / 2, y - nh / 2, nw, nh);
        g.setColor(fg);
        g.drawOval(x - nw / 2, y - nh / 2, nw, nh);
        g.setColor(fg);
        FontMetrics fm = g.getFontMetrics();
        Rectangle2D rect = fm.getStringBounds(s, g);
        g.drawString(s, x - (int) (rect.getWidth() / 2), (int) (y + rect.getHeight() / 2));
    }

    public Dimension getPreferredSize() {
        // should make this depend on the tree ...
        return new Dimension(minWidth, heightPerLevel * (10 - 1) + inset * 2);
    }

    public TreeView showTree(String title) {
        new JEasyFrame(this, title);
        return this;
    }
}