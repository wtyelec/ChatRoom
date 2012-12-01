import java.awt.*;
import java.awt.event.*;
import java.awt.Point;

class MyWindow extends Frame 	// 自定义窗口类
{
	private static final long serialVersionUID = 1L;
	SnakePanel 	panel_snake;
	Panel 		panel_tip;
	
	MyWindow()
	{
		super("GreedSnake");				// 调用父类的构造函数:赋窗口名	 
		panel_snake = new SnakePanel();					 
		panel_tip = new Panel();			
		panel_tip.add(new Label("按上下左右键控制蛇行动 空格暂停"));
		add(panel_tip,BorderLayout.NORTH);
		add(panel_snake,BorderLayout.CENTER);
		setBounds(100,100,610,500);
		setVisible(true);
		try
		{
			panel_snake.thread.start(); //开始线程
			panel_snake.validate();
		}
		catch(Exception ee){}
		addWindowListener(
							new WindowAdapter()
							{
								public void windowClosing(WindowEvent e)
								{
									System.exit(0);
								}
							}
						);
	}
}

public class Snake
{
	public static void main(String[] args)
	{
		new MyWindow();
	}
}

class SnakePanel extends Panel implements KeyListener,Runnable		// 自定义面板类，继承了键盘和线程接口
{
	private static final long serialVersionUID = 1L;
	boolean flag_over = true; 			// 判定游戏是否结束
	boolean flag_pause = true;			// 暂停标志位
	int 	node_num = 0; 				// 蛇的节数
	int 	food_rand; 					// 食物数组
	Button 	snake[]; 					// 定义蛇按钮
	Thread 	thread; 					// 定义线程	
	Point 	head_loc;					// 蛇头位置
	Point 	food_loc; 					// 食物位置
	direction dir = direction.RIGHT; 	// 蛇移动方向					
	private static final int SNAKE_MAX_LENGTH = 70;	// 蛇的最大长度 static:全局 final:const 直接引用
	
	SnakePanel()		// 构造函数:创建实例时对数据成员的初始化
	{
		//！每个对象都需要new,否则调用时会抛空指针 NullPointerException
		snake = new Button[SNAKE_MAX_LENGTH];
		thread = new Thread(this);					// 线程实例化
		head_loc = new Point();						// Point的默认构造函数:x=y=0;
		food_loc = new Point();
		for(int i = 0; i < SNAKE_MAX_LENGTH; i++)
		{
			snake[i] = new Button();
		}
		food_rand = (int)(Math.random() * 99);		// 定义两位随机数
		// 尽量减少类型转换的开销
		food_loc.x = (food_rand / 10) * 60; 		// 十位:横坐标
		food_loc.y = (food_rand % 10) * 40; 		// 个位:纵坐标
		add(snake[0]);								// 添加蛇节点
		snake[0].addKeyListener(this); 				// 为蛇头添加键盘监视器
		snake[0].setBounds(0,0,10,10);				// 设置蛇节点初始化位置和大小
		setLayout(null);							// 设置布局 	
		setBackground(Color.black);					// 设置背景
	}
	
	private void eat_food()
	{
		snake[0].setLocation(head_loc.x, head_loc.y);					// 设置蛇头位置
		if(head_loc.x == food_loc.x && head_loc.y == food_loc.y) 		// 吃到食物
		{
			node_num++;
			food_rand = (int)(Math.random() * 99);
			food_loc.x = (food_rand / 10) * 60;
			food_loc.y = (food_rand % 10) * 40;
			repaint(); 													// 重绘视图
			add(snake[node_num]); 										// 添加蛇节点
			snake[node_num].setBounds(snake[node_num-1].getBounds());	// 设置所添加蛇节点位置
		}
	}
	
	public void paint(Graphics g) 		// 在面板上绘图
	{
		g.setColor(Color.orange);
		g.fillOval(food_loc.x, food_loc.y, 10, 10);				// 食物
		g.drawString("score: " + node_num, 250, 300);			// 分数
		
		if(flag_over == false)
		{
			g.drawString("GAME OVER!", 250, 200);				// 输出游戏失败
		}
		else if(flag_over == false)
		{
			g.drawString("WIN!", 250, 200);						// 输出游戏成功
		}
	}
	
	public void run() 			// implement the inherited abstract method Runnable.run() 
	{
		while(flag_over)
		{
			if(flag_pause)
			{			
				if(dir == direction.RIGHT)		// 检测到按下右键
				{			
					head_loc.x += 10;
				}
				else if(dir == direction.LEFT)
				{
					head_loc.x -= 10;
				}
				else if(dir == direction.UP)
				{
					head_loc.y -= 10;
				}
				else if(dir == direction.DOWN)
				{
					head_loc.y += 10;
				}
				eat_food();
				try
				{
					Thread.sleep(100);		// 延时100MS
				}
				catch(Exception e)
				{
					System.out.println("eat_food exception!");
				}
				
				int num = node_num;
				while(num > 0)			
				{
					snake[num].setBounds(snake[num-1].getBounds());					// 设置除头结点的其他节点位置
					if(snake[num].getBounds().x == snake[0].getBounds().x && num >3
							&& snake[num].getBounds().y == snake[0].getBounds().y)	// 判断是否咬自己的尾巴
					{
						flag_over = false;
						repaint();
					}
					num--;
				}
				if(head_loc.x < 0 || head_loc.x >= this.getWidth()
						|| head_loc.y < 0 || head_loc.y >= this.getHeight())	// 判断是否撞墙
				{
					flag_over = false;
					repaint();
				}
			}
			System.out.println("flag_pause =" + flag_pause);
		}
	}

	public void keyPressed(KeyEvent e) 							// 按键按下瞬间
	{
		if(e.getKeyCode() == KeyEvent.VK_RIGHT)
		{
			if(dir != direction.LEFT && dir != direction.RIGHT)		// 左右移动
			{
				dir = direction.RIGHT;
			}
		}
		else if(e.getKeyCode() == KeyEvent.VK_LEFT)
		{
			if(dir != direction.LEFT && dir != direction.RIGHT)		// 左右移动
			{
				dir = direction.LEFT;
			}
		}		
		else if(e.getKeyCode() == KeyEvent.VK_UP)
		{
			if(dir != direction.UP && dir != direction.DOWN)		// 上下移动
			{
				dir = direction.UP;
			}
		}
		else if(e.getKeyCode() == KeyEvent.VK_DOWN)
		{
			if(dir != direction.UP && dir != direction.DOWN)		// 上下移动
			{
				dir = direction.DOWN;
			}
		}
		else if (e.getKeyCode() == KeyEvent.VK_ENTER)					// 重玩游戏
		{
			new MyWindow();
		}
		else if(e.getKeyCode() == KeyEvent.VK_SPACE)					// 暂停游戏
		{
			flag_pause = !flag_pause;
		}
	}
	
	public void keyTyped(KeyEvent e){}			// 按键按住中
	
	public void keyReleased(KeyEvent e){}		// 按键释放瞬间
	
	public enum direction			// 按键方向
	{  
		UP, DOWN, LEFT, RIGHT  
	} 
}
