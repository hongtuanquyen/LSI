using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Threading;
using System.Diagnostics;


namespace Main
{
    public partial class LSIVN_Hi_Draw_画面作成ツール : Form
    {
        /******************  VARIABLE *****************/
        #region VARIABLES and TYPES
        const int TYPE_DISPLAY = -1;
        const int TYPE_BUTTON = 1000;
        const int TYPE_TEXT = 2000;
        const int BUTTON_TREE = 3000;
        const int TEXT_TREE = 4000;

        enum LANG
        {
            ENGLISH = 0,
            JAPANESE
        }

        enum TOGGLE_BUTTON_STATE
        {
            ON,
            OFF,
            PRESSED
        }

        // Panel temp;
        int old_disp_id;
        const int NG = 256;

        const int CREATE = 0;
        const int EDIT = 1;

        bool Mouse_is_down;
        //bool Allow_drag;
        Point DragStart;
        Size Old_size;
        int Drag_Resize;
        const int DRAGGING = 0;
        const int RESIZING_TOPLEFT = 1;
        const int RESIZING_TOPRIGHT = 2;
        const int RESIZING_BOTTOMRIGHT = 3;
        const int RESIZING_BOTTOMLEFT = 4;
        const int RESIZE_TOLERANCE = 10;
        /// <summary>
        /// Bien can cho preview
        /// </summary>
        Form prv_form;
        /// <summary>
        /// Bien can cho preview
        /// </summary>
        public bool flag_preview;
        public bool flag_pos_max_change;
        public int display_no_preview;
        public int type_before_preview;
        /// <summary>
        /// Bien can cho copy va paste
        /// </summary>
        public int index_copy;
        public int type_copy;
        public int type_paste;
        public int flag_display_paste;
        public object[] object_copy;
        static ID_NUMBER[] item_copy;
        Display[] display_copy;
        /// <summary>
        /// Bien can cho shift va resize
        /// </summary>
        public int ctrl_Left_Old, ctrl_Top_Old;
        public bool flag_resize;
        int state_key;
        const int DEFAULT = -1;
        const int UP = 0;
        const int DOWN = 1;
        const int LEFT = 2;
        const int RIGHT = 3;
        public bool shift;
        public bool ctrl_key;
        /// <summary>
        /// B?t khi dang set c當 gitr? cho item m?i
        /// </summary>
        public bool flag_is_setting;

        /// <summary>
        /// Bien can cho import kiem tra set ngon ngu
        /// </summary>
        public bool flag_japanese;
        /// <summary>
        /// B?t n?u cb?t k? ch?nh s?a th麥 b?t n瀰
        /// </summary>
        public bool modified;

        public bool flag_import;

        public int flag_display_edit;
        public int size_correct;        //$Q: bien kiem tra size cua display,button,text co vuot qua yeu cau khong.
        public int import_fail;
        public int action;
        public int display_import_count;
        public int index_display;
        public int display_no;
        public int disp_id; // for display function
        public int count_display_delete;
        public int number_display;
        Panel[] GroupboxList;
        /// <summary>
        /// luu lai control da duoc chon
        /// </summary>
        Control selected_ctrl;
        public int ctrl_preview_no_temp;
        //GroupBox[] BackupGroupboxList;
        // public int widthBefore = 0;
        //public int heightBefore = 0;
        public bool ctrl_is_highlighted;

        //array
        public int temp_display = 1;
        public int temp_item;

        //Import
        public int type_import;

        //FormWindowState LastWindowState = FormWindowState.Minimized;

        // temp - 
        //FIXME: dg 1 b? 3 s? th!, d?i ki?u th瀟h int
        public string text_color_on_r;
        public string text_color_on_g;
        public string text_color_on_b;
        public string text_color_off_r;
        public string text_color_off_g;
        public string text_color_off_b;
        public string text_color_press_r;
        public string text_color_press_g;
        public string text_color_press_b;
        public string back_color_r;
        public string back_color_g;
        public string back_color_b;


        // Drag 'n Drop
        Rectangle dragBoxFromMouseDown;
        //  public int max_id_display;
        struct Display
        {
            //public int max_id_button;
            //  public int max_id_text;

            public int pos_import_button;
            public int pos_import_textbox;
            public int[] id_import_button;
            public int[] id_import_textbox;

            public string name;
            public int use;
            public object disp;
            public int type_item;
            public int temp_number_button;
            public int temp_number_text;
            public int count_button_delete;
            public int count_text_delete;
            public int[] button_delete;
            public int[] text_delete;
            public int[] ctrl_preview_no;

            public int size_width;
            public int size_height;


            public int flag_state;
            public bool Allow_drag;

            public int[] id_import;
            public int pos_import;

            public int id_count;
            public int id_no;
            public int type;



            //   public int return_id_no;
            public int number_button;
            public int number_text;
            public int index_button;
            public int index_text;

            public ID_NUMBER[] item_button;
            public ID_NUMBER[] item_textbox;
            public TreeNode Display_tree;
            public TreeNode Button_tree;
            public TreeNode Text_tree;
            public int limit_button;
            public int limit_text;

            public SizeF Disp_Scale_ratio;
            public SizeF Disp_Scale_ratio_recip;


        }
        static Display[] display;

        struct ID_NUMBER
        {
            public PRO_PER prop;


        }

        //  static ID_NUMBER[] item;
        struct PRO_PER
        {
            public string led_on_color;
            public string led_off_color;
            public int on_screen;
            public object item_disp;
            /// <summary>
            /// bi?n luu v? trth? t? trong m?ng c?a ctrl khi display
            /// </summary>
            public int pos_pre_no;
            public float size_text;
            public int use;
            //  int _location_x;
            public int location_x;

            public int location_y;

            public int size_height;
            public int size_width;
            public string text_jpn;

            public string text_eng;

            public int color_r;

            public int color_b;

            public int color_g;

            public int font_color_off_r;
            public int font_color_off_b;
            public int font_color_off_g;

            public int font_color_press_r;
            public int font_color_press_g;
            public int font_color_press_b;

            public int font_color_on_r;
            public int font_color_on_g;
            public int font_color_on_b;

            public string image_on;
            public string image_off;
            public string image_press;

            public string align_text;
            public int item_pos;
            /// <summary>
            /// whether button is toggle button or not?
            /// </summary>
            public bool is_toggle_button;
            public TOGGLE_BUTTON_STATE button_state;

            /// <summary>
            /// check whether textbox have border line or not.
            /// </summary>
            public bool is_label;
            public bool led;
        }
        #endregion

        #region FORM_LOAD
        public LSIVN_Hi_Draw_画面作成ツール()
        {
            InitializeComponent();

        }

        private void LSIVN_Hi_Draw_画面作成ツール_Load(object sender, System.EventArgs e)
        {
            flag_resize = true;
            Pos_Txt.Minimum = 1;
            object_copy = new object[1];
            item_copy = new ID_NUMBER[1];
            display_copy = new Display[1];

            action = 2;// ko phai ACTION & EDIT
            size_correct = 1;
            flag_display_edit = 1;
            //flag_is_setting = 1;
            display_no = 0;
            modified = false;
            temp_display = 1;
            temp_item = 0;
            prop_display(0);  // ???
            display = new Display[2]; // $Q: display[0] khong su dung
            Create_item(1);
            Btnm_Init();   // $Q: Chuc nang lam mo cac button va menu khi form moi vua load
            prop_display(-1);


            ctrl_preview_no_temp = 0;
            // Allow_drag = true;
            GroupboxList = new Panel[2]; // Panel[0] ko su dung 
            this.MinimumSize = new Size(1020, 600); // size form

            selected_ctrl = null;
            this.Text = "Untitled - LSIVN Hi-Draw - 画面作成ツール";


        }


        #endregion

        /******************** FUNCTIONS *****************************/
        // public int number_button=0;
        //public int number_text= 0;

        /*******************FUNCTION LIST****************************/
        #region FUNCTION LIST
        /*  private void list_get_info()
        {

            id_count = number_button + number_text - temp_number_button - temp_number_button - 2;
            // item[1].prop.location_x = 1;
            //id_no[id].prop.id_no
            //id_no[id].prop.location_x = ;
            //id_no[id].prop.location_y = ;
            //id_no[id].prop.size_height = 1;
            //id_no[id].prop.size_width = 1;
            //id_no[id].prop.text_jpn = 1;
            //id_no[id].prop.text_eng = 1;
            //id_no[id].prop.color_r = 1;
            //id_no[id].prop.color_b = 1;
            //id_no[id].prop.color_g = 1;

        }*/

        /// <summary>
        /// Luu item m?i t?o (display,text hay button) v瀰 Item Tree
        /// </summary>
        private void list_save_request()
        {
            int temp_id;
            if (display[display_no].type == TYPE_BUTTON)
            {
                if (Txt_display.Text != "")
                {
                    if ((int.Parse(Txt_Height.Text) <= display[display_no].size_height) && (int.Parse(Txt_Width.Text) <= display[display_no].size_width))
                    {
                        if ((int.Parse(Txt_X.Text) + int.Parse(Txt_Width.Text)) <= display[display_no].size_width && (int.Parse(Txt_Y.Text) + int.Parse(Txt_Height.Text)) <= display[display_no].size_height && int.Parse(Txt_Width.Text) != 0 && int.Parse(Txt_Height.Text) != 0)
                        {
                            display[display_no].index_button++;
                            display[display_no].number_button++;
                            Array.Resize(ref display[display_no].item_button, display[display_no].index_button + 1);

                            temp_id = display[display_no].index_button;
                            Txt_ID.Text = temp_id.ToString();
                            display[display_no].item_button[temp_id].prop.use = 1;
                            size_correct = 1;
                            flag_pos_max_change = true;
                            Pos_Txt.Maximum = display[display_no].number_button + display[display_no].number_text;
                            flag_pos_max_change = false;
                            //  display[display_no].max_id_button++;       

                        }
                        else
                        {
                            MessageBox.Show("Refill Location or Size");
                            size_correct = 0;
                        }
                    }
                    else
                    {
                        MessageBox.Show("Refill size");
                        size_correct = 0;
                    }
                }
            }
            else if (display[display_no].type == TYPE_TEXT)
            {
                if (Txt_display.Text != "")
                {
                    if ((int.Parse(Txt_Height.Text) <= display[display_no].size_height) && (int.Parse(Txt_Width.Text) <= display[display_no].size_width))
                    {
                        if ((int.Parse(Txt_X.Text) + int.Parse(Txt_Width.Text)) <= display[display_no].size_width && (int.Parse(Txt_Y.Text) + int.Parse(Txt_Height.Text)) <= display[display_no].size_height && int.Parse(Txt_Width.Text) != 0 && int.Parse(Txt_Height.Text) != 0)
                        {

                            display[display_no].index_text++;
                            display[display_no].number_text++;
                            Array.Resize(ref display[display_no].item_textbox, display[display_no].index_text + 1);

                            temp_id = display[display_no].index_text;
                            display[display_no].item_textbox[temp_id].prop.use = 1;
                            Txt_ID.Text = temp_id.ToString();
                            size_correct = 1;
                            flag_pos_max_change = true;
                            Pos_Txt.Maximum = display[display_no].number_button + display[display_no].number_text;
                            flag_pos_max_change = false;

                        }
                        else
                        {
                            MessageBox.Show("Refill Location or Size");
                            size_correct = 0;
                        }
                    }
                    else
                    {
                        MessageBox.Show("Refill size");
                        size_correct = 0;
                    }
                }
            }
            else if (display[display_no].type == TYPE_DISPLAY)
            {
                if ((int.Parse(Txt_Height.Text) < 5001) && (int.Parse(Txt_Width.Text) < 5001) && (int.Parse(Txt_Height.Text) != 0) && (int.Parse(Txt_Width.Text) != 0))
                {
                    for (; ; )
                    {

                        //treeView1.Nodes.Add();
                        number_display++;
                        index_display++;
                        display_no++;
                        temp_display++; // temp_display dc khoi tao = 1, va luon lon hon number_display vi mang dc resize chua luon display[0].
                        Array.Resize(ref display, temp_display);
                        Create_item(number_display);
                        //  Array.Resize(ref display[display_no].item, temp_item
                        display[number_display].name = "display " + index_display;
                        display[number_display].Button_tree = new TreeNode("Button");
                        display[number_display].Text_tree = new TreeNode("Text");

                        TreeNode[] array = new TreeNode[] { display[number_display].Button_tree, display[number_display].Text_tree };
                        display[number_display].Display_tree = new TreeNode(display[number_display].name, array);
                        treeView1.Nodes.Add(display[number_display].Display_tree);

                        //listBox1.Items.Add("display " + index_display);
                        temp_id = index_display;
                        display[number_display].use = 1;
                        display_no = list_return_display_no(display[number_display].name);
                        Txt_display.Text = display_no.ToString();
                        disp_id = display_no;
                        display[number_display].type = TYPE_DISPLAY;
                        size_correct = 1;
                        break;
                    }
                }
                else
                {
                    MessageBox.Show("Width and Height from 1 to 5000!!!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    size_correct = 0;
                }
            }
        }

        /*    private void listBox11_SelectedIndexChanged(object sender, EventArgs e)
            {
                string d = null;
                action = EDIT; 
                if (listBox1.SelectedItem != null)
                {
                    d = listBox1.SelectedItem.ToString();

                    if (d[0] == 'b')
                    {

                        display[display_no].id_no = Convert.ToInt16((d[7] - 48)) * 100 + Convert.ToInt16((d[8] - 48)) * 10 + Convert.ToInt16((d[9] - 48));
                        display[display_no].type_item = TYPE_BUTTON;
                        prop_display_edit(display_no,display[display_no].id_no);
                    }
                    else if (d[0] == 't')
                    {
                        display[display_no].id_no = Convert.ToInt16((d[5] - 48)) * 100 + Convert.ToInt16((d[6] - 48)) * 10 + Convert.ToInt16((d[7] - 48));
                        display[display_no].type_item = TYPE_TEXT;
                        prop_display_edit(display_no, display[display_no].id_no);

                    }
                    else if (d[0]=='d')
                    {
                        display[display_no].type_item = TYPE_DISPLAY;
                    
                        display_no = Convert.ToInt16((d[8] - 48));
                        display[display_no].id_no = display_no;
                        prop_disp_disp_edit(display_no);
                    }
                

                }
            }*/

        private void list_Selected_item_tree(object sender, TreeViewEventArgs e)
        {
            string d = null;
            string parent;

            TreeNode Selected;
            string temp;
            if (treeView1.SelectedNode != null)
            {

                d = treeView1.SelectedNode.Text;

                Selected = treeView1.SelectedNode;

                if (d[0] == 'b')
                {

                    disp_normal_state();
                    old_disp_id = display_no;
                    parent = Selected.Parent.Parent.Text;
                    Match match = Regex.Match(parent, @"(\d+)");
                    if (match.Success)
                    {
                        temp = (match.Groups[1].Value);
                        display_no = int.Parse(temp);
                        // input = input.Substring(0, input.Length - temp.Length);
                    }

                    display_no = list_return_display_no(parent);
                    flag_pos_max_change = true;
                    Pos_Txt.Maximum = display[display_no].number_button + display[display_no].number_text;
                    flag_pos_max_change = false;
                    // display_no = Convert.ToInt16((parent[8] - 48));
                    disp_show_current_display();
                    match = Regex.Match(Selected.Text, @"(\d+)");
                    if (match.Success)
                    {
                        temp = (match.Groups[1].Value);
                        display[display_no].id_no = int.Parse(temp);
                    }
                    display[display_no].type = TYPE_BUTTON;
                    prop_display_edit(display_no, TYPE_BUTTON, display[display_no].item_button[display[display_no].id_no].prop.item_disp);
                    //while(treeView1.SelectedNode.IsSelected==true)
                    disp_select_ctrl(display_no, display[display_no].id_no, TYPE_BUTTON);
                    Txt_display.Text = display_no.ToString();
                    type_copy = TYPE_BUTTON;
                    // Disp_ctrl_preview_no(display_no, display[display_no].id_no);

                    type_before_preview = 100;//TYPE BUTTON
                }
                else if (d[0] == 'B')
                {
                    disp_normal_state();
                    old_disp_id = display_no;
                    parent = Selected.Parent.Text;
                    Match match = Regex.Match(parent, @"(\d+)");
                    if (match.Success)
                    {
                        temp = (match.Groups[1].Value);
                        display_no = int.Parse(temp);
                        // input = input.Substring(0, input.Length - temp.Length);
                    }
                    display_no = list_return_display_no(parent);
                    flag_pos_max_change = true;
                    Pos_Txt.Maximum = display[display_no].number_button + display[display_no].number_text;
                    flag_pos_max_change = false;
                    //display_no = Convert.ToInt16((parent[8] - 48));
                    display[display_no].type = BUTTON_TREE;
                    disp_show_current_display();
                    prop_reset();
                    prop_display(-1);
                    Txt_display.Text = display_no.ToString();
                    Txt_ID.Text = parent[8].ToString();
                }
                else if (d[0] == 't')
                {
                    disp_normal_state();
                    old_disp_id = display_no;
                    parent = Selected.Parent.Parent.Text;
                    Match match = Regex.Match(parent, @"(\d+)");
                    if (match.Success)
                    {
                        temp = (match.Groups[1].Value);
                        display_no = int.Parse(temp);
                        // input = input.Substring(0, input.Length - temp.Length);
                    }
                    display_no = list_return_display_no(parent);
                    flag_pos_max_change = true;
                    Pos_Txt.Maximum = display[display_no].number_button + display[display_no].number_text;
                    flag_pos_max_change = false;
                    old_disp_id = display_no;
                    disp_show_current_display();

                    match = Regex.Match(Selected.Text, @"(\d+)");
                    if (match.Success)
                    {
                        temp = (match.Groups[1].Value);
                        display[display_no].id_no = int.Parse(temp);
                    }
                    display[display_no].type = TYPE_TEXT;
                    prop_display_edit(display_no, TYPE_TEXT, display[display_no].item_textbox[display[display_no].id_no].prop.item_disp);
                    disp_select_ctrl(display_no, display[display_no].id_no, TYPE_TEXT);
                    Txt_display.Text = display_no.ToString();
                    type_copy = TYPE_TEXT;
                    // Disp_ctrl_preview_no(display_no, display[display_no].id_no);

                    type_before_preview = 200; //TYPE TEXT
                }
                else if (d[0] == 'T')
                {
                    disp_normal_state();
                    old_disp_id = display_no;
                    parent = Selected.Parent.Text;
                    Match match = Regex.Match(parent, @"(\d+)");
                    if (match.Success)
                    {
                        temp = (match.Groups[1].Value);
                        display_no = int.Parse(temp);
                        // input = input.Substring(0, input.Length - temp.Length);
                    }
                    display_no = list_return_display_no(parent);
                    flag_pos_max_change = true;
                    Pos_Txt.Maximum = display[display_no].number_button + display[display_no].number_text;
                    flag_pos_max_change = false;
                    display[display_no].type = TEXT_TREE;
                    //display_no = Convert.ToInt16((parent[8] - 48));
                    disp_show_current_display();
                    prop_reset();
                    prop_display(-1);
                    Txt_display.Text = display_no.ToString();
                    Txt_ID.Text = parent[8].ToString();
                }
                else if (d[0] == 'd')
                {
                    disp_normal_state();
                    display[display_no].type_item = TYPE_DISPLAY;
                    disp_normal_state();
                    old_disp_id = display_no;
                    display_no = list_return_display_no(d);
                    flag_pos_max_change = true;
                    Pos_Txt.Maximum = display[display_no].number_button + display[display_no].number_text;
                    flag_pos_max_change = false;
                    disp_show_current_display();    // show new selected display
                    display[display_no].type = TYPE_DISPLAY;
                    display[display_no].id_no = display_no;
                    prop_disp_disp_edit(display_no);
                    type_copy = TYPE_DISPLAY;
                    Txt_ID.Text = display_no.ToString();

                    type_before_preview = 1; // TYPE DISPLAY
                    Pos_Txt.ResetText();
                    /* if (flag_preview == true)
                         prv_form.BringToFront();*/
                }

                if (display_no_preview == display_no)
                {
                    Btn_Display.Enabled = false; Menu_Edit_create_display.Enabled = false;
                    Btn_IMPORT.Enabled = false; Menu_File_load.Enabled = false;
                    Btn_Button.Enabled = false; Menu_Edit_add_button.Enabled = false;
                    Btn_Text.Enabled = false; Menu_Edit_add_text.Enabled = false;
                    Btn_Export.Enabled = false; Menu_File_save.Enabled = false;
                    Btn_Preview.Enabled = false; Menu_View_preview.Enabled = false;
                    Btn_Clear.Enabled = false; Menu_Edit_clear.Enabled = false;
                    Btn_Delete.Enabled = false; Menu_Edit_delete_item.Enabled = false;
                    Btn_Copy.Enabled = false; Menu_Edit_copy.Enabled = false;
                    Btn_Paste.Enabled = false; Menu_Edit_paste.Enabled = false;
                    Btn_Set.Enabled = false;
                    Menu_View_English.Enabled = false; Menu_View_Japanese.Enabled = false;
                    prop_disable_all();
                }
                else
                {
                    //Btnm_enable()
                    Btn_Copy.Enabled = true; Menu_Edit_copy.Enabled = true;
                    Btn_Paste.Enabled = true; Menu_Edit_paste.Enabled = true;
                    Btn_Delete.Enabled = true; Menu_Edit_delete_item.Enabled = true;
                    Btn_Clear.Enabled = true; Menu_Edit_clear.Enabled = true;
                    Btn_Export.Enabled = true; Menu_File_save.Enabled = true;
                    Btn_Preview.Enabled = true; Menu_View_preview.Enabled = true;
                    Btn_Button.Enabled = true; Menu_Edit_add_button.Enabled = true;
                    Btn_Text.Enabled = true; Menu_Edit_add_text.Enabled = true;

                    Menu_View_English.Enabled = true; Menu_View_Japanese.Enabled = true;

                    Btn_Display.Enabled = true; Menu_Edit_create_display.Enabled = true;
                    Btn_Set.Enabled = true;
                    Btn_IMPORT.Enabled = true; Menu_File_load.Enabled = true;
                }

            }
            action = EDIT;
        }
        int list_return_display_no(string name)
        {
            int i;
            for (i = 1; i <= number_display; i++)
            {
                if (name == display[i].name)
                    break;
            }
            return i;
        }


        int list_return_item_no(string name, int type)
        {
            int i = 0;
            if (type == TYPE_BUTTON)
            {
                Button btn = new Button();
                for (i = 1; i <= display[display_no].number_button; i++)
                {
                    btn = (Button)display[display_no].item_button[i].prop.item_disp;
                    if (name == btn.Name)
                        break;
                }
            }
            else if (type == TYPE_TEXT)
            {
                Label txt = new Label();
                for (i = 1; i <= display[display_no].number_text; i++)
                {
                    txt = (Label)display[display_no].item_textbox[i].prop.item_disp;
                    if (name == txt.Name)
                        break;
                }
            }
            return i;
        }
        /* private void list_clear()
         {

             for (int i = 1; i < listBox1.Items.Count; i++)
             {

                 listBox1.Items.Remove(listBox1.Items[i]);
                 i--;
             }
             display[display_no].number_button = 1;
             display[display_no].number_text = 1;
             display[display_no].temp_number_text = 0;
             display[display_no].temp_number_button = 0;
             display_no = 0;
             number_display = 0;
         }*/
        private void list_clear()
        {
            treeView1.Nodes.Clear();
            /* int count = 0;
             int count_btn = 0;
             int count_txt = 0;*/
            /*for (int i = 1; count != number_display; i++)
            {
                if (display[i].use == 1)
                {
                    display[i].number_button = 0;
                    display[i].number_text = 0;
                    display[i].temp_number_text = 0;
                    display[i].temp_number_button = 0;
                    display[i].pos_import = 0;
                    display[i].id_count = 0;
                    display[i].use = 0;
                    count_btn = 0;
                    count_txt = 0;

                    for (int j = 0; count_btn != display[i].number_button; j++)
                    {
                        if (display[i].item[100 + j].prop.on_screen != 0)
                        {
                            display[i].item[100 + j].prop.on_screen = 0;
                            count_btn++;
                        }
                    }
                    for (int j = 0; count_txt != display[i].number_text; j++)
                    {
                        if (display[i].item[200 + j].prop.on_screen != 0)
                        {
                            display[i].item[200 + j].prop.on_screen = 0;
                            count_txt++;
                        }
                    }
                    count++;
                }
            }*/
            Array.Clear(display, 0, number_display + 1);
            Array.Resize(ref display, 2);
            //disp_clear();
            index_display = 0;
            display_no = 0;
            number_display = 0;
            prop_default(TYPE_DISPLAY);
            prop_default(TYPE_BUTTON);
            prop_default(TYPE_TEXT);
        }
        /*      private void list_delete()
              {

                  int temp_delete;
                  if (listBox1.SelectedItem != null)
                  {
                      string s = listBox1.SelectedItem.ToString();
                      if (s[0] == 'b')
                      {
                          temp_delete = Convert.ToInt16((s[7] - 48)) * 100 + Convert.ToInt16((s[8] - 48)) * 10 + Convert.ToInt16((s[9] - 48));
                          display[display_no].temp_number_button = display[display_no].temp_number_button + 1;
                          display[display_no].item[display[display_no].id_no].prop.use = 0;
                          display[display_no].button_delete[display[display_no].count_button_delete + 1] = temp_delete;
                          display[display_no].number_button--;
                      }
                      else if (s[0] == 't')
                      {
                          temp_delete = Convert.ToInt16((s[5] - 48)) * 100 + Convert.ToInt16((s[6] - 48)) * 10 + Convert.ToInt16((s[7] - 48));
                          display[display_no].temp_number_text = display[display_no].temp_number_text + 1;
                          display[display_no].item[display[display_no].id_no].prop.use = 0;
                          display[display_no].text_delete[display[display_no].count_text_delete + 1] = temp_delete;
                          display[display_no].number_text--;
                      }
                      if (s[0] != 'd')
                          number_display--;
                      listBox1.Items.Remove(listBox1.SelectedItem);
                  }
              }*/
        private void list_delete()
        {
            int display_delete;
            int temp_delete = 0;
            if (treeView1.SelectedNode != null && treeView1.SelectedNode.Text != "Button" && treeView1.SelectedNode.Text != "Text")
            {
                TreeNode Selected = treeView1.SelectedNode;
                //TreeNode Change_pos;
                string temp;
                string s = Selected.Text;
                Match match = Regex.Match(s, @"(\d+)");
                if (match.Success)
                {
                    temp = (match.Groups[1].Value);
                    temp_delete = int.Parse(temp);
                    // input = input.Substring(0, input.Length - temp.Length);
                }
                if (s[0] == 'b')
                {
                    string Parent = Selected.Parent.Parent.Text;
                    display_delete = list_return_display_no(Parent);
                    if (temp_delete == display[display_delete].index_button)
                        display[display_delete].index_button--;
                    //temp_delete = Convert.ToInt16((s[7] - 48)) * 100 + Convert.ToInt16((s[8] - 48)) * 10 + Convert.ToInt16((s[9] - 48));
                    display[display_delete].temp_number_button = display[display_delete].temp_number_button + 1;
                    display[display_delete].item_button[temp_delete].prop.use = 0;
                    display[display_delete].item_button[temp_delete].prop.on_screen = 0;
                    display[display_delete].button_delete[display[display_delete].count_button_delete + 1] = temp_delete;
                    // Disp_ctrl_preview_no(display_delete, temp_delete);             
                    display[display_delete].number_button--;
                    flag_pos_max_change = true;
                    Pos_Txt.Maximum = display[display_delete].number_button + display[display_delete].number_text;
                    flag_pos_max_change = false;
                    //Pos_Txt.Value = 0;
                }
                else if (s[0] == 't')
                {
                    string Parent = Selected.Parent.Parent.Text;
                    display_delete = list_return_display_no(Parent);
                    if (temp_delete == display[display_delete].index_text)
                        display[display_delete].index_text--;
                    //temp_delete = Convert.ToInt16((s[5] - 48)) * 100 + Convert.ToInt16((s[6] - 48)) * 10 + Convert.ToInt16((s[7] - 48));
                    display[display_delete].temp_number_text = display[Parent[8] - 48].temp_number_text + 1;
                    display[display_delete].item_textbox[temp_delete].prop.use = 0;
                    display[display_delete].item_textbox[temp_delete].prop.on_screen = 0;
                    display[display_delete].text_delete[display[display_delete].count_text_delete + 1] = temp_delete;
                    // Disp_ctrl_preview_no(display_delete, temp_delete);
                    display[display_delete].number_text--;
                    flag_pos_max_change = true;
                    Pos_Txt.Maximum = display[display_delete].number_button + display[display_delete].number_text;
                    flag_pos_max_change = false;
                    //Pos_Txt.Value = 0;
                }
                if (s[0] == 'd')
                {
                    if (temp_delete == index_display)
                    {
                        index_display--;
                    }
                    //temp_delete = Convert.ToInt16((s[8] - 48));
                    temp_delete = list_return_display_no(s);

                    Array.Copy(display, temp_delete + 1, display, temp_delete, number_display - temp_delete);
                    /*display[temp_delete].number_button = 0;
                    display[temp_delete].number_text = 0;
                    display[temp_delete].temp_number_text = 0;
                    display[temp_delete].temp_number_button = 0;
                    display[temp_delete].pos_import = 0;
                    display[temp_delete].id_count = 0;
                    display[temp_delete].use = 0;*/

                    Array.Resize(ref display, number_display);
                    number_display--;
                    /*Change_pos = Selected.NextNode;
                    //treeView1.Nodes.Remove(Selected);

                    for (; Change_pos != null; )
                    {
                        match = Regex.Match(Change_pos.Text, @"(\d+)");
                        if (match.Success)
                        {
                            temp = (match.Groups[1].Value);
                            temp_delete = int.Parse(temp) - 1;
                            // input = input.Substring(0, input.Length - temp.Length);
                        }
                        Change_pos.Text = "display " + temp_delete.ToString();
                        Change_pos = Change_pos.NextNode;
                    }*/
                    //index_display--;
                }

                treeView1.SelectedNode = null;
                treeView1.Nodes.Remove(Selected);

            }
        }

        public void list_import()
        {
            int j;
            int i;

            /*    display[display_no].number_button = 0;
                display[display_no].number_text = 0;
                display[display_no].count_button_delete = 0;
                display[display_no].count_text_delete = 0;
                display[display_no].temp_number_button = 0;
                display[display_no].temp_number_text = 0;
                display[display_no].index_button = 0;
                display[display_no].index_text = 0;*/
            for (j = 1; j <= number_display; j++)
            {
                if (display[j].use == 1)
                {
                    //add node display
                    // number_display++;
                    display[j].Button_tree = new TreeNode("Button");
                    display[j].Text_tree = new TreeNode("Text");

                    TreeNode[] array = new TreeNode[] { display[j].Button_tree, display[j].Text_tree };
                    display[j].Display_tree = new TreeNode(display[j].name, array);
                    treeView1.Nodes.Add(display[j].Display_tree);

                    for (i = 0; i < display[j].pos_import_button; i++)
                    {
                        //add node button
                        //listBox1.Items.Add("button " + display[j].id_import[i].ToString());
                        if (Menu_View_English.Checked == true)
                            display[j].Button_tree.Nodes.Add("button " + display[j].id_import_button[i].ToString() + " " + display[j].item_button[display[j].id_import_button[i]].prop.text_eng);
                        else
                            display[j].Button_tree.Nodes.Add("button " + display[j].id_import_button[i].ToString() + " " + display[j].item_button[display[j].id_import_button[i]].prop.text_jpn);
                        display[j].number_button = display[j].number_button + 1;
                        display[j].item_button[display[j].id_import_button[i]].prop.use = 1;
                        display[j].item_button[display[j].id_import_button[i]].prop.on_screen = 0;

                        //display[j].ctrl_preview_no[199 - display[j].number_text - display[j].number_button + display[j].item[display[j].id_import[i]].prop.pos_pre_no] = display[j].id_import[i];
                        //disp_disp_Create_Edit(display[j].id_import[i]);             
                    }

                    for (i = 0; i < display[j].pos_import_textbox; i++)
                    {
                        //add node text
                        //listBox1.Items.Add("text " + display[j].id_import[i].ToString());
                        if (Menu_View_English.Checked == true)
                            display[j].Text_tree.Nodes.Add("textbox " + display[j].id_import_textbox[i].ToString() + " " + display[j].item_textbox[display[j].id_import_textbox[i]].prop.text_eng);
                        else
                            display[j].Text_tree.Nodes.Add("textbox " + display[j].id_import_textbox[i].ToString() + " " + display[j].item_textbox[display[j].id_import_textbox[i]].prop.text_jpn);
                        display[j].number_text = display[j].number_text + 1;
                        display[j].item_textbox[display[j].id_import_textbox[i]].prop.use = 1;
                        display[j].item_textbox[display[j].id_import_textbox[i]].prop.on_screen = 0;

                        //display[j].ctrl_preview_no[199 - display[j].number_text - display[j].number_button + display[j].item[display[j].id_import[i]].prop.pos_pre_no] = display[j].id_import[i];
                        //disp_disp_Create_Edit(display[j].id_import[i]);

                    }
#if null
                   for (i = 0; i < display[j].id_count; i++)
                        display[j].ctrl_preview_no[199 - display[j].number_text - display[j].number_button + display[j].item[display[j].id_import[i]].prop.pos_pre_no] = display[j].id_import[i];
#endif
                }
                flag_pos_max_change = true;
                Pos_Txt.Maximum = display[j].number_text + display[j].number_button;
                flag_pos_max_change = false;
            }



        }

        /// <summary>
        /// ch?n item tr麩 treeview vdi?n c當 properties
        /// </summary>
        /// <param name="ctrl_id">item id d? ch?n</param>
        /*Hien thi item duoc chon tren LIST*/
        private void list_select_item(int type, object ctrl)
        {
            TreeNode selected;

            // set available item attributes on properties
            //prop_display_edit(display_no, ctrl_id);
            if (type == TYPE_BUTTON)
            {
                int ctrl_id = Disp_return_id_no(ctrl);
                type_before_preview = 100;
                try
                {
                    selected = display[display_no].Display_tree.FirstNode.FirstNode;
                    for (; selected != null; )
                    {
                        if (Menu_View_English.Checked == true)
                        {
                            if (selected.Text != "button " + ctrl_id + " " + display[display_no].item_button[ctrl_id].prop.text_eng)
                            {
                                selected = selected.NextNode;
                            }
                            else
                                break;
                        }
                        else if (Menu_View_English.Checked == false)
                        {
                            if (selected.Text != "button " + ctrl_id + " " + display[display_no].item_button[ctrl_id].prop.text_jpn)
                            {
                                selected = selected.NextNode;
                            }
                            else
                                break;
                        }
                    }
                    treeView1.SelectedNode = selected;
                    treeView1.Select();
                }
                catch { }
            }
            if (type == TYPE_TEXT)
            {
                int ctrl_id = Disp_return_id_no(ctrl);
                type_before_preview = 200;
                try
                {
                    selected = display[display_no].Display_tree.LastNode.FirstNode;
                    for (; selected != null; )
                    {
                        if (Menu_View_English.Checked == true)
                        {
                            if (selected.Text != "textbox " + ctrl_id + " " + display[display_no].item_textbox[ctrl_id].prop.text_eng)
                            {
                                selected = selected.NextNode;
                            }
                            else
                                break;
                        }
                        else if (Menu_View_English.Checked == false)
                        {
                            if (selected.Text != "textbox " + ctrl_id + " " + display[display_no].item_textbox[ctrl_id].prop.text_jpn)
                            {
                                selected = selected.NextNode;
                            }
                            else
                                break;
                        }
                    }
                    treeView1.SelectedNode = selected;
                    treeView1.Select();
                }
                catch { }
            }
        }

        #endregion

        /*******************FUNCTION BUTTON and MENU*****************/
        #region FUNCTION BUTTON and MENU
        /// <summary>
        /// Kh?i t?o c當 button ? tr?ng th疂 b?t d?u (chua cdisplay n瀰 du?c t?o)
        /// </summary>
        private void Btnm_Init()
        {
            Btn_Display.Enabled = true; Menu_Edit_create_display.Enabled = true;
            Btn_IMPORT.Enabled = true; Menu_File_load.Enabled = true;
            Btn_Button.Enabled = false; Menu_Edit_add_button.Enabled = false;
            Btn_Text.Enabled = false; Menu_Edit_add_text.Enabled = false;
            //Btn_Image.Enabledd = false;
            //Btn_Label.Enabledd = false;
            Btn_Export.Enabled = false; Menu_File_save.Enabled = false;
            Btn_Preview.Enabled = false; Menu_View_preview.Enabled = false;
            Btn_Clear.Enabled = false; Menu_Edit_clear.Enabled = false;
            Btn_Delete.Enabled = false; Menu_Edit_delete_item.Enabled = false;
            Btn_Copy.Enabled = false; Menu_Edit_copy.Enabled = false;
            Btn_Paste.Enabled = false; Menu_Edit_paste.Enabled = false;
            Btn_Set.Enabled = true;

            Menu_View_English.Enabled = false; Menu_View_Japanese.Enabled = false;
        }

        private void Btnm_Buttons_Click(object sender, EventArgs e)
        {

            //Button sender = (Button)sender;

            //status_message(clickedButton.Text + " clicked!");

            // Display
            if (sender == Btn_Display || sender == Menu_Edit_create_display)
            {
                action = CREATE;

                flag_is_setting = false;
                //display_no ++;
                //  listBox1.Items.Add("display");
                //btnm_enable();
                display[display_no].type = TYPE_DISPLAY;
                prop_display(2);  // $Q: Ham lam mo cho vung Properties 
                prop_reset();     // $Q: xoa het noi dung text trong vung Properties
                Txt_display.Clear();
                prop_default(TYPE_DISPLAY); // Ham tra ve gia tri default(Width, Height) cho Display
                treeView1.SelectedNode = null;
                // display_no = -1;
            }
            // Button
            else if (sender == Btn_Button || sender == Menu_Edit_add_button)
            {
                Btnm_Create_button_prepare();
            }
            // Text
            else if (sender == Btn_Text || sender == Menu_Edit_add_text)
            {
                Btnm_Create_text_prepare();
            }
            // PREVIEW
            else if (sender == Btn_Preview || sender == Menu_View_preview)
            {
                flag_is_setting = false;
                /*$Q: Doan lenh if so sanh Allow_drag == false va display_no_preview != 0 la du vi khi thuc hien ham disp_preview, form moi xuat hien va cac button deu disable va khi form preview dong, Allow_drag = true va display_no_preview = 0*/
                if (display[display_no_preview].Allow_drag == false && display_no_preview != 0)
                {
                    prv_form.Close();
                    display[display_no_preview].Allow_drag = true;
                }
                disp_preview(display_no);
            }

            // EXPORT
            else if (sender == Btn_Export || sender == Menu_File_save)
            {
                //list_get_info();
                flag_is_setting = false;
                Expo_Xuat();
            }
            // IMPORT
            else if (sender == Btn_IMPORT || sender == Menu_File_load)
            {
                if (number_display != 0)
                {
                    if (MessageBox.Show("Do you want to save your current work?", "Warning", MessageBoxButtons.YesNo) == DialogResult.Yes)
                    {
                        flag_is_setting = false;
                        Expo_Xuat();
                    }
                }
                import_fail = 0;
                flag_import = true;
                flag_is_setting = false;
                Expo_Nhap();
                if (import_fail != 1)
                {
                    Btnm_enable();
                    if (flag_japanese == true)
                    {
                        Menu_View_Japanese.Checked = true;
                        Menu_View_English.Checked = false;
                    }
                    else
                    {
                        Menu_View_English.Checked = true;
                        Menu_View_Japanese.Checked = false;
                    }
                    list_import();
                    //display[display_no].Allow_drag = false;
                    disp_import();
                }
                flag_import = false;
                //display[display_no].Allow_drag = true;
            }
            // CLEAR
            else if (sender == Btn_Clear || sender == Menu_Edit_clear)
            {
                if (MessageBox.Show("Are you sure you want to clear all?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)
                {
                    /*$Q: Doan lenh if so sanh Allow_drag == false va display_no_preview != 0 la du vi khi thuc hien ham disp_preview sau khi click button Preview, form moi xuat hien va cac button deu disable va khi form preview dong, Allow_drag = true va display_no_preview = 0*/
                    if (display[display_no_preview].Allow_drag == false && display_no_preview != 0)
                    {
                        prv_form.Close();
                        display[display_no_preview].Allow_drag = true;
                    }
                    flag_is_setting = false;
                    Btnm_Init();
                    //Menu_Init_menus();
                    disp_clear();
                    list_clear();
                    prop_reset();
                    Txt_display.Clear();
                    prop_display(0);
                    action = 2;
                }
            }
            // DELETE
            else if (sender == Btn_Delete || sender == Menu_Edit_delete_item)
            {
                if (MessageBox.Show("Are you sure you want to delete this item?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                {
                    flag_is_setting = false;
                    try
                    {
                        if (treeView1.SelectedNode != null && treeView1.SelectedNode.Text != "Button" && treeView1.SelectedNode.Text != "Text")
                        {
                            string s;
                            s = treeView1.SelectedNode.ToString(); //$Q: Khac voi treeView1.SelectedNode.Text. No se tra ve chuoi ko co chu "TreeNode"
                            disp_disp_Delete(display_no); // $Q: Xoa item tren display
                            list_delete();  // delete item on list
                            //display[display_no].flag_state = 1;
                            // delete item on preview pane
                            prop_reset();
                            // Txt_display.Clear();
                            //CONFIGURE BUTTON
                            if (number_display == 0)
                            {
                                Btnm_Init();
                            }
                            prop_display(-1);
                            if (s[10] == 'd')
                                display_no--;
                        }
                    }
                    catch { }
                }
            }
            //COPY
            else if (sender == Btn_Copy || sender == Menu_Edit_copy)
            {
                //if ((Txt_display.Text == "") || (Txt_display.Text == "" && Txt_ID.Text == ""))
                if (treeView1.SelectedNode == null)
                    MessageBox.Show("Item is not selected", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                else
                {
                    if (display[display_no].type == TYPE_DISPLAY)
                        Array.Copy(display, display_no, display_copy, 0, 1);
                    else
                    {
                        if (display[display_no].type == TYPE_BUTTON)
                            Array.Copy(display[display_no].item_button, display[display_no].id_no, item_copy, 0, 1);
                        else if (display[display_no].type == TYPE_TEXT)
                            Array.Copy(display[display_no].item_textbox, display[display_no].id_no, item_copy, 0, 1);
                    }
                    //object_copy[0] = disp_clone_ctrl(display_no,display[display_no].id_no );
                    if (display[display_no].type == TYPE_BUTTON)
                    {
                        type_paste = TYPE_BUTTON;
                        Txt_Copy.Text = ("button " + display[display_no].id_no + '-' + display_no);
                    }
                    else if (display[display_no].type == TYPE_TEXT)
                    {
                        type_paste = TYPE_TEXT;
                        Txt_Copy.Text = ("text " + display[display_no].id_no + '-' + display_no);
                    }
                    else if (display[display_no].type == TYPE_DISPLAY)
                    {
                        type_paste = TYPE_DISPLAY;
                        Txt_Copy.Text = (display[display_no].name);
                    }
                    else if (display[display_no].type == BUTTON_TREE)
                    {
                        type_paste = BUTTON_TREE;
                        index_copy = display_no;
                        Txt_Copy.Text = "Btn tree" + index_copy;
                    }
                    else if (display[display_no].type == TEXT_TREE)
                    {
                        type_paste = TEXT_TREE;
                        index_copy = display_no;
                        Txt_Copy.Text = "Txt tree" + index_copy;
                    }
                }
            }
            //PASTE
            else if (sender == Btn_Paste || sender == Menu_Edit_paste)
            {
                int number_btn_in_tree;
                int number_txt_in_tree;
                if (Txt_Copy.Text == "")
                    MessageBox.Show("Item is not copy", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                else
                {
                    if (type_paste == TYPE_BUTTON)
                    {
                        Button btn = (Button)object_copy[0];
                        if (item_copy[0].prop.size_width + item_copy[0].prop.location_x > display[display_no].size_width)
                        {
                            item_copy[0].prop.location_x = 0;
                            if (item_copy[0].prop.size_width > display[display_no].size_width)
                                item_copy[0].prop.size_width = display[display_no].size_width;
                        }
                        if (item_copy[0].prop.size_height + item_copy[0].prop.location_y > display[display_no].size_height)
                        {
                            item_copy[0].prop.location_y = 0;
                            if (item_copy[0].prop.size_height > display[display_no].size_height)
                                item_copy[0].prop.size_height = display[display_no].size_height;
                        }
                        display[display_no].index_button++;
                        display[display_no].number_button++;
                        Array.Resize(ref display[display_no].item_button, display[display_no].index_button + 1);
                        Copy_item(display_no, display[display_no].index_button, item_copy[0], TYPE_BUTTON);

                        if (Menu_View_English.Checked == true)
                            display[display_no].Button_tree.Nodes.Add("button " + display[display_no].index_button + " " + display[display_no].item_button[display[display_no].index_button].prop.text_eng);
                        else
                            display[display_no].Button_tree.Nodes.Add("button " + display[display_no].index_button + " " + display[display_no].item_button[display[display_no].index_button].prop.text_jpn);

                        display[display_no].item_button[display[display_no].index_button].prop.item_pos = display[display_no].number_text + display[display_no].number_button;
                        disp_ctrl_create_edit(display_no, display[display_no].index_button, TYPE_BUTTON);
                        treeView1.SelectedNode = display[display_no].Button_tree.LastNode;
                        treeView1.Select();
                        /*}
                        else
                            MessageBox.Show("Can't copy, width and height of display < width and height item");*/

                    }
                    else if (type_paste == TYPE_TEXT)
                    {
                        Label lbl = (Label)object_copy[0];
                        if (item_copy[0].prop.size_width + item_copy[0].prop.location_x > display[display_no].size_width)
                        {
                            item_copy[0].prop.location_x = 0;
                            if (item_copy[0].prop.size_width > display[display_no].size_width)
                                item_copy[0].prop.size_width = display[display_no].size_width;
                        }
                        if (item_copy[0].prop.size_height + item_copy[0].prop.location_y > display[display_no].size_height)
                        {
                            item_copy[0].prop.location_y = 0;
                            if (item_copy[0].prop.size_height > display[display_no].size_height)
                                item_copy[0].prop.size_height = display[display_no].size_height;
                        }
                        display[display_no].type = TYPE_TEXT;
                        display[display_no].index_text++;
                        display[display_no].number_text++;
                        Array.Resize(ref display[display_no].item_textbox, display[display_no].index_text + 1);
                        Copy_item(display_no, display[display_no].index_text, item_copy[0], TYPE_TEXT);

                        if (Menu_View_English.Checked == true)
                            display[display_no].Text_tree.Nodes.Add("textbox " + display[display_no].index_text + " " + display[display_no].item_textbox[display[display_no].index_text].prop.text_eng);
                        else
                            display[display_no].Text_tree.Nodes.Add("textbox " + display[display_no].index_text + " " + display[display_no].item_textbox[display[display_no].index_text].prop.text_jpn);

                        display[display_no].item_textbox[display[display_no].index_text].prop.item_pos = display[display_no].number_text + display[display_no].number_button;
                        disp_ctrl_create_edit(display_no, display[display_no].index_text, TYPE_TEXT);
                        treeView1.SelectedNode = display[display_no].Text_tree.LastNode;
                        treeView1.Select();
                        /*}
                        else
                            MessageBox.Show("Can't copy, width and height of display < width and height item");*/

                    }
                    else if (type_paste == TYPE_DISPLAY)
                    {
                        number_display++;
                        index_display++;
                        //display_no++;
                        temp_display++;
                        Array.Resize(ref display, temp_display);
                        Create_item(number_display);
                        //copy thong so tu display cu
                        Copy_display(number_display);
                        //Tao groupboxlist moi hien thi display moi
                        disp_create_disp(number_display);
                        //Hien ra treeview
                        treeView1.Nodes.Add(display[number_display].Display_tree);
                        treeView1.SelectedNode = display[number_display].Display_tree;
                        treeView1.Select();
                        //hien thi lan luot cac item trong display copy
                        int count_btn = 0;
                        int count_txt = 0;
                        for (int i = 1; count_btn < display[number_display].number_button; i++)
                        {
                            if (display[number_display].item_button[i].prop.use == 1)
                            {
                                //display[display_no].Button_tree.Nodes.Add("button " + i);
                                disp_ctrl_create_edit(number_display, i, TYPE_BUTTON);
                                count_btn++;
                            }
                        }
                        for (int i = 1; count_txt < display[number_display].number_text; i++)
                        {
                            if (display[number_display].item_textbox[i].prop.use == 1)
                            {
                                //display[display_no].Text_tree.Nodes.Add("text " + i);
                                disp_ctrl_create_edit(number_display, i, TYPE_TEXT);
                                count_txt++;
                            }
                        }

                        display[number_display].use = 1;
                        //display_no = index_display;
                        disp_id = display_no;
                        display[number_display].type = TYPE_DISPLAY;
                        size_correct = 1;
                        flag_display_paste = 0;
                    }
                    else if (type_paste == BUTTON_TREE)
                    {
                        number_btn_in_tree = display[index_copy].number_button;
                        int count = 0;
                        for (int i = 1; count < number_btn_in_tree; i++)
                        {
                            if (display[index_copy].item_button[i].prop.use == 1)
                            {
                                count++;
                                Array.Copy(display[index_copy].item_button, i, item_copy, 0, 1);
                                //Button btn = (Button)object_copy[0];
                                if (item_copy[0].prop.size_width + item_copy[0].prop.location_x > display[display_no].size_width)
                                {
                                    item_copy[0].prop.location_x = 0;
                                    if (item_copy[0].prop.size_width > display[display_no].size_width)
                                        item_copy[0].prop.size_width = display[display_no].size_width;
                                }
                                if (item_copy[0].prop.size_height + item_copy[0].prop.location_y > display[display_no].size_height)
                                {
                                    item_copy[0].prop.location_y = 0;
                                    if (item_copy[0].prop.size_height > display[display_no].size_height)
                                        item_copy[0].prop.size_height = display[display_no].size_height;
                                }
                                display[display_no].index_button++;
                                display[display_no].number_button++;
                                Array.Resize(ref display[display_no].item_button, display[display_no].index_button + 1);
                                Copy_item(display_no, display[display_no].index_button, item_copy[0], TYPE_BUTTON);

                                if (Menu_View_English.Checked == true)
                                    display[display_no].Button_tree.Nodes.Add("button " + display[display_no].index_button + " " + display[display_no].item_button[display[display_no].index_button].prop.text_eng);
                                else
                                    display[display_no].Button_tree.Nodes.Add("button " + display[display_no].index_button + " " + display[display_no].item_button[display[display_no].index_button].prop.text_jpn);

                                display[display_no].item_button[display[display_no].index_button].prop.item_pos = display[display_no].number_text + display[display_no].number_button;
                                // Copy_item(display_no, 100 + display[display_no].index_button, item_copy[0]);
                                disp_ctrl_create_edit(display_no, display[display_no].index_button, TYPE_BUTTON);
                            }
                        }

                        flag_pos_max_change = true;
                        Pos_Txt.Maximum = display[display_no].number_button + display[display_no].number_text;
                        flag_pos_max_change = false;
                        treeView1.SelectedNode = display[display_no].Button_tree;
                        treeView1.Select();
                    }
                    else if (type_paste == TEXT_TREE)
                    {
                        number_txt_in_tree = display[index_copy].number_text;
                        int count = 0;
                        for (int i = 1; count < number_txt_in_tree; i++)
                        {
                            if (display[index_copy].item_textbox[i].prop.use == 1)
                            {
                                count++;
                                Array.Copy(display[index_copy].item_textbox, i, item_copy, 0, 1);
                                // Label lbl = (Label)object_copy[0];
                                if (item_copy[0].prop.size_width + item_copy[0].prop.location_x > display[display_no].size_width)
                                {
                                    item_copy[0].prop.location_x = 0;
                                    if (item_copy[0].prop.size_width > display[display_no].size_width)
                                        item_copy[0].prop.size_width = display[display_no].size_width;
                                }
                                if (item_copy[0].prop.size_height + item_copy[0].prop.location_y > display[display_no].size_height)
                                {
                                    item_copy[0].prop.location_y = 0;
                                    if (item_copy[0].prop.size_height > display[display_no].size_height)
                                        item_copy[0].prop.size_height = display[display_no].size_height;
                                }
                                display[display_no].type = TYPE_TEXT;
                                display[display_no].index_text++;
                                display[display_no].number_text++;
                                Array.Resize(ref display[display_no].item_textbox, display[display_no].index_text + 1);
                                Copy_item(display_no, display[display_no].index_text, item_copy[0], TYPE_TEXT);

                                if (Menu_View_English.Checked == true)
                                    display[display_no].Text_tree.Nodes.Add("textbox " + display[display_no].index_text + " " + display[display_no].item_textbox[display[display_no].index_text].prop.text_eng);
                                else
                                    display[display_no].Text_tree.Nodes.Add("textbox " + display[display_no].index_text + " " + display[display_no].item_textbox[display[display_no].index_text].prop.text_jpn);

                                display[display_no].item_textbox[display[display_no].index_text].prop.item_pos = display[display_no].number_text + display[display_no].number_button;
                                //  Copy_item(display_no, 200 + display[display_no].index_text, item_copy[0]);
                                disp_ctrl_create_edit(display_no, display[display_no].index_text, TYPE_TEXT);
                            }
                        }

                        flag_pos_max_change = true;
                        Pos_Txt.Maximum = display[display_no].number_button + display[display_no].number_text;
                        flag_pos_max_change = false;
                        treeView1.SelectedNode = display[display_no].Text_tree;
                        treeView1.Select();
                    }
                }
            }
            // QUIT
            else if (sender == Menu_File_quit)
            {
                this.Close();
            }
        }

        /// <summary>
        /// Fillin default properties value for creating new button
        /// </summary>
        private void Btnm_Create_button_prepare()
        {
            flag_is_setting = true;
            prop_reset();
            prop_display(TYPE_BUTTON);
            action = CREATE;
            if (display_no <= number_display)
            {
                display[display_no].type = TYPE_BUTTON;
                prop_display(TYPE_BUTTON);
                prop_default(TYPE_BUTTON);
            }
            else
            {
                MessageBox.Show("Please select display", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                prop_display(-1);
            }
        }

        private void Btnm_Create_text_prepare()
        {
            // it's like clicking on "Text" button
            flag_is_setting = true;
            prop_reset();
            action = CREATE;
            prop_display(TYPE_TEXT);
            if (display_no <= number_display)
            {
                display[display_no].type = TYPE_TEXT;
                prop_display(TYPE_TEXT);
                prop_default(TYPE_TEXT);
            }
            else
            {
                MessageBox.Show("Please select display", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                prop_display(-1);
            }
        }

        /// <summary>
        /// Set button event handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btnm_Btn_Set_Click(object sender, EventArgs e)
        {
            int id = 0;

            if (action == CREATE)
            {
                /*Trong truong hop ko dien du thong tin control*/
                if (display[display_no].type == TYPE_DISPLAY && (Txt_Width.Text == "" || Txt_Height.Text == ""))
                {
                    MessageBox.Show("Missing information", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                }
                else if (display[display_no].type == TYPE_BUTTON && (Txt_Width.Text == "" || Txt_Height.Text == "" || Txt_X.Text == "" || Txt_Y.Text == ""))
                {

                    MessageBox.Show("Missing information", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    // full_info = 0;
                }

                else if (display[display_no].type == TYPE_TEXT && (Txt_Width.Text == "" || Txt_Height.Text == "" || Txt_X.Text == "" || Txt_Y.Text == ""))
                {
                    MessageBox.Show("Missing information", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    // full_info = 0;
                }
                /*Trong truong hop dien du thong tin control*/
                else
                {

                    old_disp_id = display_no;  // $Q: Khi tao duoc display moi, chuong trinh se hide cai display truoc do. bien old_disp_id dung de kiem tra.
                    //  display_no = index_display;

                    list_save_request();

                    if (size_correct == 1)
                    {
                        try
                        {
                            id = int.Parse(Txt_ID.Text); // ???

                        }
                        catch { }
                        try
                        {
                            int id_display = int.Parse(Txt_display.Text);
                            if (display[display_no].type == TYPE_DISPLAY)
                            {
                                prop_disp_prop(id_display);  // $Q: Kiem tra size cua display muon tao so voi cac child_control(button/text), neu ok thi save vao display[id_display].size_height va .size_width
                            }
                            else
                            {
                                if (display[display_no].type == TYPE_BUTTON)
                                {
                                    properties(display_no, TYPE_BUTTON, display[display_no].index_button);// $Q: Doi voi SET button BUTTON/TEXT ma ko fai DISPLAY, noi dung thuoc tinh duoc thiet lap tren vung PROPERTIES se duoc luu vao display[display_no].item[id]...(VD: .size_height, .size_width,.image_off...)
                                }
                                else if (display[display_no].type == TYPE_TEXT)
                                {
                                    properties(display_no, TYPE_TEXT, display[display_no].index_text);// $Q: Doi voi SET button BUTTON/TEXT ma ko fai DISPLAY, noi dung thuoc tinh duoc thiet lap tren vung PROPERTIES se duoc luu vao display[display_no].item[id]...(VD: .size_height, .size_width,.image_off...)                             
                                }
                                if (display[display_no].type == TYPE_BUTTON)
                                {
                                    //listBox1.Items.Add("button 10" + display[display_no].index_button);

                                    if (Menu_View_English.Checked == true)
                                        display[display_no].Button_tree.Nodes.Add("button " + display[display_no].index_button + " " + display[display_no].item_button[display[display_no].index_button].prop.text_eng);
                                    else
                                        display[display_no].Button_tree.Nodes.Add("button " + display[display_no].index_button + " " + display[display_no].item_button[display[display_no].index_button].prop.text_jpn);
                                }
                                else if (display[display_no].type == TYPE_TEXT)
                                {
                                    if (Menu_View_English.Checked == true)
                                        display[display_no].Text_tree.Nodes.Add("textbox " + display[display_no].index_text + " " + display[display_no].item_textbox[display[display_no].index_text].prop.text_eng);
                                    else
                                        display[display_no].Text_tree.Nodes.Add("textbox " + display[display_no].index_text + " " + display[display_no].item_textbox[display[display_no].index_text].prop.text_jpn);
                                }
                            }

                            if (display[display_no].type == TYPE_DISPLAY)
                            {

                                if (number_display == 1)  /*Khi da tao dc 1 display dau tien, cac button COPY,PASTE,CLEAR,EXPORT... se duoc enable.*/
                                {
                                    Btnm_enable();
                                }
                                disp_create_disp(display_no);
                                treeView1.SelectedNode = display[number_display].Display_tree;
                                treeView1.Select();
                            }
                            else
                            {
                                flag_is_setting = false;
                                if (display[display_no].type == TYPE_BUTTON)
                                {
                                                                       
                                    disp_ctrl_create_edit(display_no, display[display_no].index_button, TYPE_BUTTON);
                                    treeView1.SelectedNode = display[display_no].Button_tree.LastNode;
                                  
                                }
                                else if (display[display_no].type == TYPE_TEXT)
                                {
                                    disp_ctrl_create_edit(display_no, display[display_no].index_text, TYPE_TEXT);
                                    treeView1.SelectedNode = display[display_no].Text_tree.LastNode;
                                }
                                treeView1.Select();
                            }
                        }
                        catch
                        {
                            MessageBox.Show("Please select display", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                        }
                    }
                }
            }
            else if (action == EDIT)
            {
                try
                {
                    id = int.Parse(Txt_ID.Text);
                }
                catch { }

                try
                {
                    int id_display = int.Parse(Txt_display.Text);

                    if (display[display_no].type == TYPE_DISPLAY)
                        prop_disp_prop(id_display);
                    else
                    {
                        if (display[display_no].type == TYPE_BUTTON)
                        {
                            properties(display_no, TYPE_BUTTON, id);
                        }
                        else if (display[display_no].type == TYPE_TEXT)
                        {
                            properties(display_no, TYPE_TEXT, id);
                        }
                    }
                    if (display[display_no].type == TYPE_DISPLAY)
                    {
                        if (flag_display_edit == 1) // = 0 khi size cua display ko thoa yeu cau doi voi size cua tat ca child control. (xem ham prop_disp_prop().) 
                        {
                            disp_create_disp(display_no);
                            disp_calculate_ratio();                        
                            disp_scale();  // Do ratio thay doi nen cac child control cung fai resize theo dung ti le so voi display.                           
                        }
                    }
                    else
                    {
                        if (display[display_no].type == TYPE_BUTTON)
                            disp_ctrl_create_edit(display_no, id, TYPE_BUTTON);
                        else if (display[display_no].type == TYPE_TEXT)
                            disp_ctrl_create_edit(display_no, id, TYPE_TEXT);
                    }
                }
                catch { }
            }
            modified = true;
        }

        /// <summary>
        /// Help menu items handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btnm_Help_click(object sender, EventArgs e)
        {
            if (sender == Menu_Help_about)
            {
                flag_is_setting = false;
                MessageBox.Show("LSIVN Hi-Draw ver 1.0 \n http://lsi-dev.co.jp/",
                                "画面作成ツール",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            //else if (sender == Menu_Help_known_bugs)
            //{
            //    flag_is_setting = false;
            //    MessageBox.Show("*(L昱 ch?n Image khg double click du?c!)\n"+
            //                    "...",
            //                    "Nh?ng l?i chua th? kh?c ph?c!");
            //}
        }

        private void Btnm_enable()
        {
            //  Btn_Display.Enabled = false;
            //status_message("Prop_Buttons_onclick(type_display);");
            //Btn_Button.Enabled = true;
            //Btn_Text.Enabled = true;

            //Btn_Export.Enabled = true;
            //Btn_Preview.Enabled = true;
            //Btn_Clear.Enabled = true;
            Btn_Copy.Enabled = true; Menu_Edit_copy.Enabled = true;
            Btn_Paste.Enabled = true; Menu_Edit_paste.Enabled = true;
            Btn_Delete.Enabled = true; Menu_Edit_delete_item.Enabled = true;
            Btn_Clear.Enabled = true; Menu_Edit_clear.Enabled = true;
            Btn_Export.Enabled = true; Menu_File_save.Enabled = true;
            Btn_Preview.Enabled = true; Menu_View_preview.Enabled = true;
            Btn_Button.Enabled = true; Menu_Edit_add_button.Enabled = true;
            Btn_Text.Enabled = true; Menu_Edit_add_text.Enabled = true;

            Menu_View_English.Enabled = true; Menu_View_Japanese.Enabled = true;
            Menu_View_English.Checked = true; Menu_View_Japanese.Checked = false;

        }

        private void Btnm_View_EngJpn_Click(object sender, EventArgs e)
        {
            if (sender == Menu_View_English)
            {
                Menu_View_English.Checked = true; Menu_View_Japanese.Checked = false;
                disp_change_language(LANG.ENGLISH);
            }
            else if (sender == Menu_View_Japanese)
            {
                Menu_View_English.Checked = false; Menu_View_Japanese.Checked = true;
                disp_change_language(LANG.JAPANESE);
            }
        }
        #endregion

        /*******************FUNCTION DISPLAY*************************/
        #region FUNCTION DISPLAY

        private void disp_import()
        {
            int count = 0;

            for (int i = 1; count != number_display; i++)
            {
                if (display[i].use == 1)
                {
                    disp_id = i;
                    display_no = i;
                    disp_create_disp(i);
                    GroupboxList[i].Visible = false;
                    count++;
                    for (int j = 1; j <= display[i].number_button + display[i].number_text; j++)
                    {
                        int item_id = Disp_return_item_id(i, j);
                        if (display[i].index_button >= item_id)
                        {
                            if (display[i].item_button[item_id].prop.item_pos == j)
                                disp_ctrl_create_edit(i, item_id, TYPE_BUTTON);
                        }
                        if (display[i].index_text >= item_id)
                        {
                            if (display[i].item_textbox[item_id].prop.item_pos == j)
                                disp_ctrl_create_edit(i, item_id, TYPE_TEXT);
                        }
                    }
                }
            }
            //Hien thi display dau tien, lam sang tren tree
            for (int i = 1; i <= 20; i++)
            {
                try
                {
                    if (display[i].use == 1)
                    {
                        GroupboxList[i].Visible = true;
                        treeView1.SelectedNode = display[i].Display_tree;
                        treeView1.Select();
                        break;
                    }
                }
                catch { }
            }
        }

        private int Disp_return_item_id(int disp_id, int item_pos)
        {
            int item_id = 0;
            int flag = 0;
            int index_btn = 0;
            int index_txt = 0;
            for (int i = 1; i <= display[disp_id].number_button; i++)
            {
                item_id = index_btn + i;
                if (display[disp_id].item_button[item_id].prop.use != 0)
                {
                    if (item_pos == display[disp_id].item_button[item_id].prop.item_pos)
                    {
                        flag = 1;
                        return item_id;
                    }
                }
                else
                {
                    index_btn++;
                    i--;

                }
            }
            if (flag != 1)
            {
                for (int i = 1; i <= display[disp_id].number_text; i++)
                {
                    item_id = index_txt + i;
                    if (display[disp_id].item_textbox[item_id].prop.use != 0)
                    {
                        if (item_pos == display[disp_id].item_textbox[item_id].prop.item_pos)
                        {
                            return item_id;

                        }
                    }
                    else
                    {
                        index_txt++;
                        i--;

                    }
                }
            }

            return item_id;
        }



        private void Disp_item_pos(int disp_id)
        {
            int item_id;
            Control ctrl;
            int item_total = display[disp_id].number_text + display[disp_id].number_button;
            for (int i = 1; i <= item_total; i++)
            {

                item_id = Disp_return_item_id(disp_id, i);
                if (display[disp_id].index_button >= item_id)
                {
                    if (display[disp_id].item_button[item_id].prop.item_pos == i)
                    {
                        if (display[disp_id].item_button[item_id].prop.use != 0)
                        {
                            ctrl = (Control)display[disp_id].item_button[item_id].prop.item_disp;
                            ctrl.BringToFront();
                        }
                    }
                }
                if (display[disp_id].index_text >= item_id)
                {
                    if (display[disp_id].item_textbox[item_id].prop.item_pos == i)
                    {
                        if (display[disp_id].item_textbox[item_id].prop.use != 0)
                        {
                            ctrl = (Control)display[disp_id].item_textbox[item_id].prop.item_disp;
                            ctrl.BringToFront();
                        }
                    }
                }

            }


        }


        private void disp_calculate_ratio()
        {
            GroupboxList[display_no] = (Panel)display[display_no].disp;

            //   display[display_no].item[0].prop.size_width = 100;
            display[display_no].Disp_Scale_ratio = new SizeF((float)display[display_no].size_width / GroupboxList[display_no].Width,
                                  (float)display[display_no].size_height / GroupboxList[display_no].Height);
            display[display_no].Disp_Scale_ratio_recip = new SizeF(1 / display[display_no].Disp_Scale_ratio.Width, 1 / display[display_no].Disp_Scale_ratio.Height);

        }

        private int Disp_return_id_no(object sender)
        {
            String s;
            int id_ret = NG;
            Control ctrl;
            ctrl = (Control)sender;
            //s = ctrl.Name.ToCharArray();   
            Match match = Regex.Match(ctrl.Name, @"(\d+)");
            if (match.Success)
            {
                s = (match.Groups[1].Value);
                id_ret = int.Parse(s);

                //input = input.Substring(0, input.Length - temp.Length);
            }
            return id_ret;

        }

        private void Disp_ctrl_preview_no(int disp_id, int ctrl_id)
        {
            try
            {
                int i, j, k;
                i = 199 - (display[disp_id].number_button + display[disp_id].number_text) + 1;
                for (j = i; j <= 199; j++)
                {
                    if (display[disp_id].ctrl_preview_no[j] == ctrl_id)
                    {
                        ctrl_preview_no_temp = display[disp_id].ctrl_preview_no[j];
                        break;
                    }
                }
                for (k = j; k > i; k--)
                {
                    display[disp_id].ctrl_preview_no[k] = display[disp_id].ctrl_preview_no[k - 1];

                }
                display[disp_id].ctrl_preview_no[i] = ctrl_preview_no_temp;
            }
            catch { }
        }

        private void Disp_ctrl_Mouse_Down(object sender, MouseEventArgs e)
        {
            //

            int ctrl_id;
            Control ctrl = (Control)sender;
            ctrl_id = Disp_return_id_no(ctrl);

            action = EDIT;
            // ctrl.Visible = true; 

            //btn = (Button)item[id_no].prop.item_disp;
            /*$Q: Vi image chi co o Button*/
            if (ctrl is Button)
            {
                display[display_no].type = TYPE_BUTTON;
                Button btn = (Button)ctrl;
                Button btn_first_form = (Button)display[display_no].item_button[ctrl_id].prop.item_disp;
                prop_display_edit(display_no, TYPE_BUTTON, btn);
                list_select_item(TYPE_BUTTON, btn);
                if (btn.Name.Length == btn_first_form.Name.Length)
                    this.BringToFront();
                prop_disable_all();

                if (display[display_no].item_button[ctrl_id].prop.image_press != null)
                {
                    try
                    {
                        Image img = Image.FromFile(@display[display_no].item_button[ctrl_id].prop.image_press);
                        Bitmap bmp = new Bitmap(img, ctrl.Width, ctrl.Height);
                        btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                    }
                    catch { }
                }
                btn.ForeColor = Color.FromArgb(display[display_no].item_button[ctrl_id].prop.font_color_press_r,
                                display[display_no].item_button[ctrl_id].prop.font_color_press_g,
                                display[display_no].item_button[ctrl_id].prop.font_color_press_b);
                //display[display_no].item[ctrl_id].prop.button_state = TOGGLE_BUTTON_STATE.PRESSED;
            }

            else if (ctrl is Label)
            {
                display[display_no].type = TYPE_TEXT;
                Label txt = (Label)ctrl;
                Label txt_first_form = (Label)display[display_no].item_textbox[ctrl_id].prop.item_disp;
                prop_display_edit(display_no, TYPE_TEXT, txt);
                list_select_item(TYPE_TEXT, txt);
                if (txt.Name.Length == txt_first_form.Name.Length)
                    this.BringToFront();
                prop_disable_all();
            }

            if (display[display_no].Allow_drag == true && !flag_is_setting)
            {
                // get dragsize
                Size dragSize = SystemInformation.DragSize;

                // Create a rectangle using the DragSize, with the mouse position being
                // at the center of the rectangle.                         
                dragBoxFromMouseDown = new Rectangle(new Point(e.X - (dragSize.Width / 2),
                                                               e.Y - (dragSize.Height / 2)),
                                                     dragSize);
                //label1.Text = e.X + "," + e.Y;

                //disp_select_ctrl(display_no, ctrl_id);
                if (ctrl is Button)
                {
                    Button btn = (Button)ctrl;
                    prop_display_edit(display_no, TYPE_BUTTON, btn);
                }
                else if (ctrl is Label)
                {
                    Label txt = (Label)ctrl;
                    prop_display_edit(display_no, TYPE_TEXT, txt);
                }
                Old_size = ctrl.Size;
                ctrl_Left_Old = ctrl.Left;
                ctrl_Top_Old = ctrl.Top;
                //ctrl.BringToFront();
                // Disp_ctrl_preview_no(display_no, ctrl_id);
                DragStart = new Point(e.X, e.Y);
                //Cursor.Clip = this.RectangleToScreen(new Rectangle(GroupboxList[display_no].Left + this.Left,
                //    GroupboxList[display_no].Top + this.Location.Y,
                //    GroupboxList[display_no].Width, GroupboxList[display_no].Height));
                //Cursor.Clip = this.RectangleToScreen(new Rectangle(GroupboxList[display_no].Location, GroupboxList[display_no].Size));
                ctrl.Capture = true;

                Mouse_is_down = true;
                if (e.X <= RESIZE_TOLERANCE && e.Y <= RESIZE_TOLERANCE)
                    Drag_Resize = RESIZING_TOPLEFT;
                else if (e.X >= ctrl.Width - RESIZE_TOLERANCE && e.Y <= RESIZE_TOLERANCE)
                    Drag_Resize = RESIZING_TOPRIGHT;
                else if (e.X >= ctrl.Width - RESIZE_TOLERANCE && e.Y >= ctrl.Height - RESIZE_TOLERANCE)
                    Drag_Resize = RESIZING_BOTTOMRIGHT;
                else if (e.X <= RESIZE_TOLERANCE && e.Y >= ctrl.Height - RESIZE_TOLERANCE)
                    Drag_Resize = RESIZING_BOTTOMLEFT;
                else
                    Drag_Resize = DRAGGING;
            }

            if (flag_is_setting)
            {
                if (MessageBox.Show("Do you want to cancel the setting", "Warning", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    flag_is_setting = false;
                }
                else
                {
                    flag_is_setting = true;
                    action = CREATE;
                }
            }


        }

        private void Disp_ctrl_Mouse_Up(object sender, MouseEventArgs e)
        {
            int ctrl_id;
            Control ctrl = (Control)sender;
            ctrl_id = Disp_return_id_no(ctrl);
            Console.WriteLine("******************UP************************");
            Console.WriteLine("ctrl_id: " + ctrl_id);
            /*            if (ctrl_id > 1000)
                        {
                            display_no = ctrl_id / 1000;
                            ctrl_id = ctrl_id % 1000;

                            prop_display_edit(display_no, ctrl_id);
                            prop_disable_all();

                        }
            */

            if (ctrl is Button)
            {
                Button btn = (Button)ctrl;

                if (display[display_no].item_button[ctrl_id].prop.is_toggle_button)
                {
                    //TOGGLE_BUTTON_STATE old_btn_state = display[display_no].item[ctrl_id].prop.button_state;
                    if (display[display_no].item_button[ctrl_id].prop.button_state == TOGGLE_BUTTON_STATE.OFF)
                        display[display_no].item_button[ctrl_id].prop.button_state = TOGGLE_BUTTON_STATE.ON;
                    else if (display[display_no].item_button[ctrl_id].prop.button_state == TOGGLE_BUTTON_STATE.ON)
                        display[display_no].item_button[ctrl_id].prop.button_state = TOGGLE_BUTTON_STATE.OFF;

                    if (display[display_no].item_button[ctrl_id].prop.button_state == TOGGLE_BUTTON_STATE.ON)
                    {
                        if (display[display_no].item_button[ctrl_id].prop.image_on != null)
                        {
                            try
                            {
                                Image img = Image.FromFile(@display[display_no].item_button[ctrl_id].prop.image_on);
                                Bitmap bmp = new Bitmap(img, ctrl.Width, ctrl.Height);
                                btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                            }

                            catch { }
                        }
                        btn.ForeColor = Color.FromArgb(display[display_no].item_button[ctrl_id].prop.font_color_on_r,
                                                        display[display_no].item_button[ctrl_id].prop.font_color_on_g,
                                                        display[display_no].item_button[ctrl_id].prop.font_color_on_b);

                    }
                    else if (display[display_no].item_button[ctrl_id].prop.button_state == TOGGLE_BUTTON_STATE.OFF)
                    {
                        if (display[display_no].item_button[ctrl_id].prop.image_off != null)
                        {
                            try
                            {
                                Image img = Image.FromFile(@display[display_no].item_button[ctrl_id].prop.image_off);
                                Bitmap bmp = new Bitmap(img, ctrl.Width, ctrl.Height);
                                btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                            }

                            catch { }
                        }
                        btn.ForeColor = Color.FromArgb(display[display_no].item_button[ctrl_id].prop.font_color_off_r,
                                                        display[display_no].item_button[ctrl_id].prop.font_color_off_g,
                                                        display[display_no].item_button[ctrl_id].prop.font_color_off_b);
                    }
                }// is toggle button
                else
                {
                    if (display[display_no].item_button[ctrl_id].prop.image_off != null)
                    {
                        try
                        {
                            Image img = Image.FromFile(@display[display_no].item_button[ctrl_id].prop.image_off);
                            Bitmap bmp = new Bitmap(img, ctrl.Width, ctrl.Height);
                            btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                        }
                        catch { }
                    }
                    btn.ForeColor = Color.FromArgb(display[display_no].item_button[ctrl_id].prop.font_color_off_r,
                                                        display[display_no].item_button[ctrl_id].prop.font_color_off_g,
                                                        display[display_no].item_button[ctrl_id].prop.font_color_off_b);
                }
            }

            // cap nhat toa do

            if (display[display_no].Allow_drag == true && !flag_is_setting)
            {
                //Cursor.Clip = Screen.PrimaryScreen.Bounds;
                // $Q: Doan code if duoi chi cap nhat gia tri x,y moi vao display[display_no].... 
                if (Drag_Resize == DRAGGING)
                {

                    float x = ctrl.Location.X * display[display_no].Disp_Scale_ratio.Width;
                    float y = ctrl.Location.Y * display[display_no].Disp_Scale_ratio.Height;
                    try
                    {
                        float compare_x = Math.Abs(Convert.ToInt32(Txt_X.Text) - x);
                        float compare_y = Math.Abs(Convert.ToInt32(Txt_Y.Text) - y);

                        //disp_refine_ctrl_position(ctrl);
                        //if (/*n?u dragStart ve.X c當h nhau d? l?n*/)
                        //if (e.X - DragStart.X > 2 && e.Y - DragStart.Y > 2)
                        //{
                        if (compare_x >= 1 || compare_y >= 1)
                        {
                            if (ctrl is Button)
                            {
                               
                                display[display_no].item_button[ctrl_id].prop.location_x = Convert.ToInt32(ctrl.Location.X * display[display_no].Disp_Scale_ratio.Width);
                                display[display_no].item_button[ctrl_id].prop.location_y = Convert.ToInt32(ctrl.Location.Y * display[display_no].Disp_Scale_ratio.Height);
                                prop_enable_disable_onchange_trigger(true);
                                Txt_X.Text = display[display_no].item_button[ctrl_id].prop.location_x.ToString();
                                Txt_Y.Text = display[display_no].item_button[ctrl_id].prop.location_y.ToString();
                                prop_enable_disable_onchange_trigger(false);
                            }
                            else if (ctrl is Label)
                            {
                                display[display_no].item_textbox[ctrl_id].prop.location_x = Convert.ToInt32(ctrl.Location.X * display[display_no].Disp_Scale_ratio.Width);
                                display[display_no].item_textbox[ctrl_id].prop.location_y = Convert.ToInt32(ctrl.Location.Y * display[display_no].Disp_Scale_ratio.Height);
                                prop_enable_disable_onchange_trigger(true);
                                Txt_X.Text = display[display_no].item_textbox[ctrl_id].prop.location_x.ToString();
                                Txt_Y.Text = display[display_no].item_textbox[ctrl_id].prop.location_y.ToString();
                                prop_enable_disable_onchange_trigger(false);
                            }
                        }
                    }
                    catch { }
                    //}
                }
                else if (Drag_Resize == RESIZING_TOPLEFT || Drag_Resize == RESIZING_TOPRIGHT || Drag_Resize == RESIZING_BOTTOMRIGHT || Drag_Resize == RESIZING_BOTTOMLEFT)
                {
                    if (ctrl.Width == 0 || ctrl.Height == 0)
                    {
                        MessageBox.Show("Width and Height must be greater than zero", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                        GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio);
                        if (ctrl is Button)
                        {
                            ctrl.Width = display[display_no].item_button[ctrl_id].prop.size_width;
                            ctrl.Height = display[display_no].item_button[ctrl_id].prop.size_height;
                            ctrl.Location = new Point(display[display_no].item_button[ctrl_id].prop.location_x, display[display_no].item_button[ctrl_id].prop.location_y);
                        }
                        else if (ctrl is Label)
                        {
                            ctrl.Width = display[display_no].item_textbox[ctrl_id].prop.size_width;
                            ctrl.Height = display[display_no].item_textbox[ctrl_id].prop.size_height;
                            ctrl.Location = new Point(display[display_no].item_textbox[ctrl_id].prop.location_x, display[display_no].item_textbox[ctrl_id].prop.location_y);

                        }
                        GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio_recip);
                    }
                    else
                    {
                        float x = ctrl.Width * display[display_no].Disp_Scale_ratio.Width;
                        float y = ctrl.Height * display[display_no].Disp_Scale_ratio.Height;
                        float compare_x = Math.Abs(Convert.ToInt32(Txt_Width.Text) - x);
                        float compare_y = Math.Abs(Convert.ToInt32(Txt_Height.Text) - y);
                        if (compare_x >= 1 || compare_y >= 1)
                        { 
                            if (ctrl is Button)
                            {
                                Console.WriteLine("ctrl.Location.X: " + ctrl.Location.X);
                                //disp_refine_ctrl_size(ctrl);

                                display[display_no].item_button[ctrl_id].prop.location_x = Convert.ToInt32(ctrl.Location.X * display[display_no].Disp_Scale_ratio.Width);
                                display[display_no].item_button[ctrl_id].prop.location_y = Convert.ToInt32(ctrl.Location.Y * display[display_no].Disp_Scale_ratio.Height);
                            
                                GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio); // SCALE UP
                                display[display_no].item_button[ctrl_id].prop.size_width = ctrl.Width;
                                display[display_no].item_button[ctrl_id].prop.size_height = ctrl.Height;
                                GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio_recip); // SCALE DOWN
                                //disp_refine_ctrl_size(ctrl);

                                prop_enable_disable_onchange_trigger(true);                    // avoid triggering Txt_TextChange event
                                Txt_X.Text = display[display_no].item_button[ctrl_id].prop.location_x.ToString();
                                Txt_Y.Text = display[display_no].item_button[ctrl_id].prop.location_y.ToString();
                                Txt_Width.Text = display[display_no].item_button[ctrl_id].prop.size_width.ToString();
                                Txt_Height.Text = display[display_no].item_button[ctrl_id].prop.size_height.ToString();
                                prop_enable_disable_onchange_trigger(false);
                            }
                            else if (ctrl is Label)
                            {

                                display[display_no].item_textbox[ctrl_id].prop.location_x = Convert.ToInt32(ctrl.Location.X * display[display_no].Disp_Scale_ratio.Width);
                                display[display_no].item_textbox[ctrl_id].prop.location_y = Convert.ToInt32(ctrl.Location.Y * display[display_no].Disp_Scale_ratio.Height);
                            
                                //disp_refine_ctrl_size(ctrl);
                                GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio); // SCALE UP
                                display[display_no].item_textbox[ctrl_id].prop.size_width = ctrl.Width;
                                display[display_no].item_textbox[ctrl_id].prop.size_height = ctrl.Height;
                                GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio_recip); // SCALE DOWN
                                //disp_refine_ctrl_size(ctrl);

                                prop_enable_disable_onchange_trigger(true);                    // avoid triggering Txt_TextChange event
                                Txt_X.Text = display[display_no].item_textbox[ctrl_id].prop.location_x.ToString();
                                Txt_Y.Text = display[display_no].item_textbox[ctrl_id].prop.location_y.ToString();
                                Txt_Width.Text = display[display_no].item_textbox[ctrl_id].prop.size_width.ToString();
                                Txt_Height.Text = display[display_no].item_textbox[ctrl_id].prop.size_height.ToString();
                                prop_enable_disable_onchange_trigger(false);
                            }

                        }
                    }
                }
                modified = true;
                ctrl.Capture = false;
                Mouse_is_down = false;
                if (ctrl is Button)
                    list_select_item(TYPE_BUTTON, ctrl);
                else if (ctrl is Label)
                    list_select_item(TYPE_TEXT, ctrl);
                // Reset the drag rectangle when the mouse button is raised.
                dragBoxFromMouseDown = Rectangle.Empty;
               
            }
            // flag_mouse_up = 1;
        }

        private void Disp_ctrl_Mouse_Move(object sender, MouseEventArgs e)
        {
            Control ctrl = (Control)sender;
            //TextBox txt = (TextBox)ctrl;

            // change cursor shape when hovering on resizing region
            if (display[display_no].Allow_drag && !flag_is_setting)
            {
                if (e.X >= ctrl.Width - RESIZE_TOLERANCE && e.Y >= ctrl.Height - RESIZE_TOLERANCE)
                {
                    ctrl.Cursor = Cursors.SizeNWSE;
                }
                else if (e.X >= ctrl.Width - RESIZE_TOLERANCE && e.Y <= RESIZE_TOLERANCE)
                {
                    ctrl.Cursor = Cursors.SizeNESW;
                }
                else if (e.X <= RESIZE_TOLERANCE && e.Y >= ctrl.Height - RESIZE_TOLERANCE)
                {
                    ctrl.Cursor = Cursors.SizeNESW;
                }
                else if (e.X <= RESIZE_TOLERANCE && e.Y <= RESIZE_TOLERANCE)
                {
                    ctrl.Cursor = Cursors.SizeNWSE;
                }
                else
                {
                    ctrl.Cursor = Cursors.Default;
                }
                if (Mouse_is_down)
                {
                    if (Drag_Resize == RESIZING_TOPLEFT)
                    {
                        ctrl.Width = Math.Max(0, Math.Min(ctrl.Right, ctrl.Width - e.X + DragStart.X));
                        ctrl.Height = Math.Max(0, Math.Min(ctrl.Bottom, ctrl.Height - e.Y + DragStart.Y));
                        ctrl.Location = new Point(Math.Min(ctrl_Left_Old + Old_size.Width, Math.Max(0, ctrl.Location.X + e.X - DragStart.X)), 
                                                  Math.Min(ctrl_Top_Old + Old_size.Height, Math.Max(0, ctrl.Location.Y + e.Y - DragStart.Y)));
                    }
                    else if (Drag_Resize == RESIZING_TOPRIGHT)
                    {
                        ctrl.Width = Math.Min(GroupboxList[display_no].Width - ctrl.Left, e.X + (Old_size.Width - DragStart.X));
                        //ctrl.Height = Math.Min(ctrl.Bottom, ctrl.Height - e.Y + DragStart.Y);
                        ctrl.Height = Math.Max(0, Math.Min(ctrl.Bottom, ctrl.Height - e.Y + DragStart.Y));
                        ctrl.Location = new Point(ctrl.Location.X, 
                                                  Math.Min(ctrl_Top_Old + Old_size.Height, Math.Max(0, ctrl.Location.Y + e.Y - DragStart.Y)));
                    }
                    else if (Drag_Resize == RESIZING_BOTTOMRIGHT)
                    {
                        ctrl.Width = Math.Min(GroupboxList[display_no].Width - ctrl.Left, e.X + (Old_size.Width - DragStart.X));
                        ctrl.Height = Math.Min(GroupboxList[display_no].Height - ctrl.Top, e.Y + (Old_size.Height - DragStart.Y));
                    }
                    else if (Drag_Resize == RESIZING_BOTTOMLEFT)
                    {
                        ctrl.Width = Math.Max(0, Math.Min(ctrl.Right, ctrl.Width - e.X + DragStart.X));
                        ctrl.Height = Math.Min(GroupboxList[display_no].Height - ctrl.Top, e.Y + (Old_size.Height - DragStart.Y));
                        ctrl.Location = new Point(Math.Min(ctrl_Left_Old + Old_size.Width, Math.Max(0, ctrl.Location.X + e.X - DragStart.X)), 
                                                  ctrl.Location.Y);
                    }
                    else if (Drag_Resize == DRAGGING)
                    {
                        //Point client
                        //Debug.Print("dragBoxFromMouseDown " + dragBoxFromMouseDown.X + "," + dragBoxFromMouseDown.Y + "," + dragBoxFromMouseDown.Width + "," + dragBoxFromMouseDown.Height);
                        //Debug.Print("dragBoxFromMouseDown " + dragBoxFromMouseDown.Contains(e.X, e.Y));

                        //if (dragBoxFromMouseDown != Rectangle.Empty && !dragBoxFromMouseDown.Contains(e.X, e.Y))
                        //{
                        //label1.Text = dragBoxFromMouseDown.Contains(e.X, e.Y) + ":" + e.X + "," + e.Y + ";" + ctrl.Left + "," + ctrl.Top;
                        // ch? cho ph駱 move theo chi?u ngang n?u c n?m trong groupbox
                        //if (ctrl.Left + ctrl.Width <= GroupboxList[display_no].Width)
                        //{                      
                        ctrl.Left = Math.Max(0, Math.Min(GroupboxList[display_no].Width - ctrl.Width, e.X + ctrl.Left - DragStart.X));  // d?ch chuy?n ctrl theo chi?u ngang
                        //}
                        //else
                        //{
                        //ctrl.Left = GroupboxList[display_no].Width - ctrl.Width;
                        //}
                        // ch? cho ph駱 move theo chi?u d?c n?u c n?m trong groupbox
                        //if (ctrl.Top + ctrl.Height <= GroupboxList[display_no].Height)
                        //{
                        ctrl.Top = Math.Max(0, Math.Min(GroupboxList[display_no].Height - ctrl.Height, e.Y + ctrl.Top - DragStart.Y));    // d?ch chuy?n ctrl theo chi?u d?c
                        //}
                        //label1.Text = "e: " + e.X + "," + e.Y + "\n" + "ctrl: " + ctrl.Left + "," + ctrl.Top + "\n" + "ctrlsize: " + ctrl.Width + "," + ctrl.Height; 
                        //}
                    }
                }
                // n?u ph疸 hi?n chu?t di chuy?n ra kh?i ctrl
                //if (Mouse_is_down == false)
                //{
                //Disp_ctrl_Mouse_Up(ctrl, e);
                //}
            }
        }
        private void Disp_Disp_Mouse_Click(object sender, MouseEventArgs e)
        {
            int disp_id;
            string name;
            Panel grp = (Panel)sender;
            disp_id = Disp_return_id_no(grp);
            name = "display " + disp_id;
            display_no = list_return_display_no(name);
            display[display_no].type = TYPE_DISPLAY;
            /* Pos_Txt.Value = 1;
                 Pos_Txt.Value =0;*/
            prop_disp_disp_edit(display_no);   // Hien thi nhung attribute cua Display.

            type_before_preview = 1;
            treeView1.SelectedNode = display[display_no].Display_tree;
            treeView1.Select();

            Txt_ID.Text = disp_id.ToString();
        }

        /// <summary>
        /// ch?nh l?i k兤h thu?c control d? khg b? l? ra ngo瀛 groupbox
        /// </summary>
        /// <param name="ctrl">control object d? ch?nh</param>
        private void disp_refine_ctrl_size(Control ctrl)
        {
            disp_refine_ctrl_position(ctrl);
            if (ctrl.Left + ctrl.Width > GroupboxList[display_no].Width)
            {
                ctrl.Width = GroupboxList[display_no].Width - ctrl.Left;
            }
            if (ctrl.Top + ctrl.Height > GroupboxList[display_no].Height)
            {
                ctrl.Height = GroupboxList[display_no].Height - ctrl.Top;
            }
        }

        /// <summary>
        /// ch?nh l?i v? trcontrol d? khg b? l? ra ngo瀛 groupbox
        /// </summary>
        /// <param name="ctrl">control object d? ch?nh</param>
        private void disp_refine_ctrl_position(Control ctrl)
        {
            if (ctrl.Left + ctrl.Width > GroupboxList[display_no].Width)
            {
                ctrl.Left = GroupboxList[display_no].Width - ctrl.Width;
            }
            if (ctrl.Top + ctrl.Height > GroupboxList[display_no].Height)
            {
                ctrl.Top = GroupboxList[display_no].Height - ctrl.Height;
            }
            if (ctrl.Left < 0)
            {
                ctrl.Left = 0;
            }
            if (ctrl.Top < 0)
            {
                ctrl.Top = 0;
            }
        }

        /// <summary>
        /// create or edit display
        /// </summary>
        /// <param name="disp_no">display_id to create/edit</param>
        private void disp_create_disp(int disp_no)
        {
            // ?n groupbox cu va hien groupbox hien tai
            if (display[old_disp_id].disp != null)
            {
                GroupboxList[old_disp_id].Visible = false;
                // GroupboxList[disp_no].Visible = true;

            }

            if (display[disp_no].disp == null)  // $Q: Truong hop chi SET lai gia tri ma khong tao moi Display, doan code trong if nay ko thuc hien do disp_no ko thay doi
            {
                Array.Resize(ref GroupboxList, temp_display);
                GroupboxList[disp_no] = new Panel();
                display[disp_no].disp = GroupboxList[disp_no];
                GroupboxList[disp_no].Location = new Point(267, 30);
                this.Controls.Add(GroupboxList[disp_no]);
                GroupboxList[disp_no].Name = "panel" + disp_no;

                GroupboxList[disp_no].MouseClick += new MouseEventHandler(Disp_Disp_Mouse_Click);

                // Drag 'n Drop
                GroupboxList[disp_no].AllowDrop = true;
                GroupboxList[disp_no].DragDrop += new DragEventHandler(disp_panel_DragDrop);
                GroupboxList[disp_no].DragOver += new DragEventHandler(disp_panel_DragOver);
            }
            if (display[disp_no].size_width == display[disp_no].size_height)
            {

                GroupboxList[disp_no].Width = 446;
                GroupboxList[disp_no].Height = 430;

            }


            else if (display[disp_no].size_width > display[disp_no].size_height)
            {
                GroupboxList[disp_no].Width = 446;
                GroupboxList[disp_no].Height = Convert.ToInt16(446f / display[disp_no].size_width * display[disp_no].size_height);

            }
            else
            {
                GroupboxList[disp_no].Height = 430;
                GroupboxList[disp_no].Width = Convert.ToInt16(430f / display[disp_no].size_height * display[disp_no].size_width);

            }
            this.Show();

            GroupboxList[disp_no].BackColor = Color.White;

            GroupboxList[disp_no].Visible = true;
            //GroupboxList[disp_no].Show();

            // GroupboxList[disp_no].Name = "groupbox" + display_no;
            //GroupboxList[disp_no].Name = "groupbox" + display_no;
            disp_calculate_ratio();
            // else

            //GroupboxList[disp_no].Show();

            
        }

        private void disp_panel_DragEnter(object sender, DragEventArgs e)
        {
            //throw new NotImplementedException();
        }

        private void disp_panel_DragOver(object sender, DragEventArgs e)
        {
            Panel pnl = (Panel)sender;
            //Point clientPoint = pnl.PointToClient(new Point(e.X, e.Y));
            e.Effect = DragDropEffects.Copy;
            //label2.Text = clientPoint.X + "," + clientPoint.Y + "\n" +
            //              clientPoint.X*Disp_Scale_ratio.Width + "," + clientPoint.Y*Disp_Scale_ratio.Height;
            //throw new NotImplementedException();
        }

        private void disp_panel_DragLeave(object sender, DragEventArgs e)
        {
            //throw new NotImplementedException();
        }

        private void disp_panel_DragDrop(object sender, DragEventArgs e)
        {
            int id;
            Panel pnl = (Panel)sender;
            Point clientPoint = pnl.PointToClient(new Point(e.X, e.Y));

            // Drop location
            prop_enable_disable_onchange_trigger(true);
            Txt_X.Text = ((int)(clientPoint.X * display[display_no].Disp_Scale_ratio.Width)).ToString();
            Txt_Y.Text = ((int)(clientPoint.Y * display[display_no].Disp_Scale_ratio.Height)).ToString();
            prop_enable_disable_onchange_trigger(false);

            // press Set button
            // Create item on display
            if (display[display_no].type == TYPE_DISPLAY && (Txt_Width.Text == "" || Txt_Height.Text == ""))
            {
                MessageBox.Show("Missing information", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            }
            else if (display[display_no].type == TYPE_BUTTON && (Txt_Width.Text == "" || Txt_Height.Text == "" || Txt_X.Text == "" || Txt_Y.Text == ""))
            {

                MessageBox.Show("Missing information", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                // full_info = 0;
            }
            else if (display[display_no].type == TYPE_TEXT && (Txt_Width.Text == "" || Txt_Height.Text == "" || Txt_X.Text == "" || Txt_Y.Text == ""))
            {
                MessageBox.Show("Missing information", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                // full_info = 0;
            }
            else
            {
                list_save_request();

                if (size_correct == 1)
                {
                    if (display[display_no].type != TYPE_DISPLAY)
                    {
                        try
                        {
                            id = int.Parse(Txt_ID.Text);
                            int id_display = int.Parse(Txt_display.Text);
                            properties(display_no, display[display_no].type, id);
                            if (display[display_no].type == TYPE_BUTTON)
                            {

                                if (Menu_View_English.Checked == true)
                                    display[display_no].Button_tree.Nodes.Add("button " + display[display_no].index_button + " " + display[display_no].item_button[display[display_no].index_button].prop.text_eng);
                                else
                                    display[display_no].Button_tree.Nodes.Add("button " + display[display_no].index_button + " " + display[display_no].item_button[display[display_no].index_button].prop.text_jpn);

                            }
                            else if (display[display_no].type == TYPE_TEXT)
                            {

                                if (Menu_View_English.Checked == true)
                                    display[display_no].Text_tree.Nodes.Add("textbox " + display[display_no].index_text + " " + display[display_no].item_textbox[display[display_no].index_text].prop.text_eng);
                                else
                                    display[display_no].Text_tree.Nodes.Add("textbox " + display[display_no].index_text + " " + display[display_no].item_textbox[display[display_no].index_text].prop.text_jpn);

                            }

                            disp_ctrl_create_edit(display_no, id, display[display_no].type);
                            // Select last Tree Item
                            try
                            {
                                if (display[display_no].type == TYPE_BUTTON)
                                    treeView1.SelectedNode = display[display_no].Button_tree.LastNode;
                                else if (display[display_no].type == TYPE_TEXT)
                                    treeView1.SelectedNode = display[display_no].Text_tree.LastNode;
                                treeView1.Select();
                            }
                            catch (Exception ex)
                            {
                                Debug.Print("Cannot select Tree item: " + ex.Message);
                            }

                            flag_is_setting = false;
                        }
                        catch (Exception)
                        {

                            MessageBox.Show("Please select display", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                        }
                    }
                }
            }
            // Reset the drag rectangle after dropped.
            dragBoxFromMouseDown = Rectangle.Empty;

        }
        /// <summary>
        /// create or edit control (item)
        /// </summary>
        /// <param name="disp_id">display id to edit/create on</param>
        /// <param name="item_id">item id to create/edit</param>
        private void disp_ctrl_create_edit(int disp_id, int item_id, int type)
        {
            //GroupBox grp;
            GroupboxList[disp_id] = (Panel)display[disp_id].disp;

            if (type == TYPE_BUTTON)     // item_id is button
            {
                Button btn;
                if (display[disp_id].item_button[item_id].prop.on_screen == 0)
                {
                    // tao moi object
                    btn = new Button();
                    // btn.Size = new Size(item[id_no].prop.size_x, item[id_no].prop.size_y);
                    display[disp_id].item_button[item_id].prop.item_disp = btn; // assign
                    display[disp_id].item_button[item_id].prop.on_screen = '1';
                    display[disp_id].item_button[item_id].prop.use = 1;
                    //GroupboxList[disp_id].Scale(Disp_Scale_ratio); // SCALE UP
                    GroupboxList[disp_id].Controls.Add(btn);
                    btn.BringToFront();
                    //GroupboxList[disp_id].Scale(Disp_Scale_ratio_recip); // SCALE UP
                    btn.Name = "button" + item_id;
                    btn.MouseDown += Disp_ctrl_Mouse_Down;
                    btn.MouseUp += Disp_ctrl_Mouse_Up;
                    btn.MouseMove += Disp_ctrl_Mouse_Move;
                    //btn.KeyDown += OnKeyDownHandler;
                    // button state
                    if (display[disp_id].item_button[item_id].prop.is_toggle_button)
                    {
                        display[disp_id].item_button[item_id].prop.button_state = TOGGLE_BUTTON_STATE.OFF;
                    }
                    // dragging
                }
                btn = (Button)display[disp_id].item_button[item_id].prop.item_disp;
                //btn.FlatStyle = FlatStyle.Flat;
                //btn.FlatAppearance.BorderColor = Color.Red;

                // btn.Width = display[disp_id].item[item_id].prop.size_width;
                // btn.Height = display[disp_id].item[item_id].prop.size_height;
                disp_calculate_ratio();
                GroupboxList[disp_id].Scale(display[disp_id].Disp_Scale_ratio); // SCALE UP

                /*btn.MaximumSize = new Size(display[display_no].item[item_id].prop.size_width, display[display_no].item[item_id].prop.size_height);
                btn.MinimumSize = new Size(display[display_no].item[item_id].prop.size_width, display[display_no].item[item_id].prop.size_height);*/
                btn.Width = display[disp_id].item_button[item_id].prop.size_width;
                btn.Height = display[disp_id].item_button[item_id].prop.size_height;
                btn.Location = new Point(display[disp_id].item_button[item_id].prop.location_x, display[disp_id].item_button[item_id].prop.location_y);
                if (Menu_View_English.Checked == true)
                    btn.Text = display[disp_id].item_button[item_id].prop.text_eng;
                else if (Menu_View_Japanese.Checked == true)
                    btn.Text = display[disp_id].item_button[item_id].prop.text_jpn;

                if (display[disp_id].item_button[item_id].prop.size_text > 0)
                {
                    btn.Font = new Font("Arial", display[disp_id].item_button[item_id].prop.size_text * display[disp_id].Disp_Scale_ratio_recip.Height);
                }// text size 
                GroupboxList[disp_id].Scale(display[disp_id].Disp_Scale_ratio_recip); // SCALE DOWN

                //if (display[disp_id].item[item_id].prop.is_toggle_button)
                //{
                //    display[disp_id].item[item_id].prop.button_state = TOGGLE_BUTTON_STATE.OFF;
                //}

                if (display[disp_id].item_button[item_id].prop.is_toggle_button && display[display_no].item_button[item_id].prop.button_state == TOGGLE_BUTTON_STATE.ON)
                {
                    btn.ForeColor = Color.FromArgb(display[disp_id].item_button[item_id].prop.font_color_on_r, display[disp_id].item_button[item_id].prop.font_color_on_g, display[disp_id].item_button[item_id].prop.font_color_on_b);
                    if (display[disp_id].item_button[item_id].prop.image_on != null)
                    {
                        try
                        {
                            Image img = Image.FromFile(@display[disp_id].item_button[item_id].prop.image_on);
                            Bitmap bmp = new Bitmap(img, btn.Width, btn.Height);
                            btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                        }
                        catch { }
                    }

                }
                else
                {
                    btn.ForeColor = Color.FromArgb(display[disp_id].item_button[item_id].prop.font_color_off_r, display[disp_id].item_button[item_id].prop.font_color_off_g, display[disp_id].item_button[item_id].prop.font_color_off_b);
                    if (display[disp_id].item_button[item_id].prop.image_off != null)
                    {
                        try
                        {
                            Image img = Image.FromFile(@display[disp_id].item_button[item_id].prop.image_off);
                            Bitmap bmp = new Bitmap(img, btn.Width, btn.Height);
                            btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                        }
                        catch { }
                    }
                }
                btn.Show();

            }
            else if (type == TYPE_TEXT)    // item_id is text
            {
                display[display_no].id_no = item_id;
                Label txt;
                if (display[disp_id].item_textbox[item_id].prop.on_screen == 0)
                {
                    txt = new Label();
                    display[disp_id].item_textbox[item_id].prop.item_disp = txt;
                    display[disp_id].item_textbox[item_id].prop.on_screen = '1';
                    GroupboxList[disp_id].Controls.Add(txt);
                    txt.BringToFront();
#if null
                    if (flag_display_paste == 0 && flag_import == false)//Dieu kien can de copy paste display ko bi sai
                    {
                        i = 199 - (display[disp_id].number_text + display[disp_id].number_button) + 1;
                        Console.WriteLine("i: " + i);
                        display[disp_id].ctrl_preview_no[i] = item_id;
                    }
#endif
                    //txt.Multiline = true;                    // Helper.ControlMover.Init(txt);
                    txt.Name = "text" + item_id;
                    //if (display[disp_id].item[item_id].prop.led)
                    //{
                    //    txt.Paint +=Disp_Txt_Paint;
                    //}
                    txt.MouseDown += Disp_ctrl_Mouse_Down;
                    txt.MouseUp += Disp_ctrl_Mouse_Up;
                    txt.MouseMove += Disp_ctrl_Mouse_Move;
                    //txt.ReadOnly = true;
                    txt.Cursor = Cursors.Default;
                    txt.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;

                }
                txt = (Label)display[disp_id].item_textbox[item_id].prop.item_disp;
                disp_calculate_ratio();
                GroupboxList[disp_id].Scale(display[disp_id].Disp_Scale_ratio); // SCALE UP
                /*txt.MaximumSize = new Size(display[disp_id].item[item_id].prop.size_width, display[disp_id].item[item_id].prop.size_height);
                txt.MinimumSize = new Size(display[disp_id].item[item_id].prop.size_width, display[disp_id].item[item_id].prop.size_height);*/
                txt.Width = display[disp_id].item_textbox[item_id].prop.size_width;
                txt.Height = display[disp_id].item_textbox[item_id].prop.size_height;
                txt.Location = new Point(display[disp_id].item_textbox[item_id].prop.location_x, display[disp_id].item_textbox[item_id].prop.location_y);
                txt.BackColor = Color.FromArgb(display[disp_id].item_textbox[item_id].prop.color_r, display[disp_id].item_textbox[item_id].prop.color_g, display[disp_id].item_textbox[item_id].prop.color_b);
                txt.Tag = txt.BackColor;
                txt.ForeColor = Color.FromArgb(display[disp_id].item_textbox[item_id].prop.font_color_off_r, display[disp_id].item_textbox[item_id].prop.font_color_off_g, display[disp_id].item_textbox[item_id].prop.font_color_off_b);
                //if (display[disp_id].item[item_id].prop.led)
                //{
                //    txt.Paint +=Disp_Txt_Paint;
                //}
                if (display[disp_id].item_textbox[item_id].prop.size_text > 0)
                {
                    txt.Font = new Font("Arial", display[disp_id].item_textbox[item_id].prop.size_text * display[disp_id].Disp_Scale_ratio_recip.Height);
                }
                if (display[disp_id].item_textbox[item_id].prop.align_text == "TopCenter")
                {
                    txt.TextAlign = ContentAlignment.TopCenter;
                }
                else if (display[disp_id].item_textbox[item_id].prop.align_text == "TopLeft")
                {
                    txt.TextAlign = ContentAlignment.TopLeft;
                }
                else if (display[disp_id].item_textbox[item_id].prop.align_text == "TopRight")
                {
                    txt.TextAlign = ContentAlignment.TopRight;
                }
                else if (display[disp_id].item_textbox[item_id].prop.align_text == "BottomCenter")
                {
                    txt.TextAlign = ContentAlignment.BottomCenter;
                }
                else if (display[disp_id].item_textbox[item_id].prop.align_text == "BottomLeft")
                {
                    txt.TextAlign = ContentAlignment.BottomLeft;
                }
                else if (display[disp_id].item_textbox[item_id].prop.align_text == "BottomRight")
                {
                    txt.TextAlign = ContentAlignment.BottomRight;
                }
                else if (display[disp_id].item_textbox[item_id].prop.align_text == "MiddleCenter")
                {
                    txt.TextAlign = ContentAlignment.MiddleCenter;
                }
                else if (display[disp_id].item_textbox[item_id].prop.align_text == "MiddleLeft")
                {
                    txt.TextAlign = ContentAlignment.MiddleLeft;
                }
                else if (display[disp_id].item_textbox[item_id].prop.align_text == "MiddleRight")
                {
                    txt.TextAlign = ContentAlignment.MiddleRight;
                }

                if (Menu_View_English.Checked == true)
                {
                    txt.Text = display[disp_id].item_textbox[item_id].prop.text_eng;
                    //txt.Text += "\r\n";
                }
                else if (Menu_View_Japanese.Checked == true)
                    txt.Text = display[disp_id].item_textbox[item_id].prop.text_jpn;
                //txt.Text = "Multi-line" + Environment.NewLine + "label";
                GroupboxList[disp_id].Scale(display[disp_id].Disp_Scale_ratio_recip); // SCALE DOWN

                // label
                if (display[disp_id].item_textbox[item_id].prop.is_label)
                {
                    txt.BorderStyle = BorderStyle.None;
                }
                else
                {
                    txt.BorderStyle = BorderStyle.FixedSingle;
                }
            }


        }

        private void disp_disp_Delete(int disp_id)
        {
            //int i;
            Control ctrl;
            GroupboxList[disp_id] = (Panel)display[disp_id].disp;
            if (display[disp_id].type == TYPE_DISPLAY)
            {

                this.Controls.Remove(GroupboxList[disp_id]);
                Array.Copy(GroupboxList, disp_id + 1, GroupboxList, disp_id, number_display - disp_id);


                // GroupboxList[disp_id].Dispose();

                /*   GroupboxList[disp_id] = null;
                   display[disp_id].disp = null;
                   for (i = 1; i <= 20; i++)
                   {
                       if (GroupboxList[i] != null)
                       {
                           old_disp_id = i;
                           display_no = i;
                           break;
                       }
                   }*/
                Array.Resize(ref GroupboxList, number_display + 1);
            }
            else
            {
                if (display[disp_id].type == TYPE_BUTTON)
                {
                    int item_id = int.Parse(Txt_ID.Text);
                    ctrl = (Control)display[disp_id].item_button[item_id].prop.item_disp;
                    GroupboxList[disp_id].Controls.Remove(ctrl);
                    display[disp_id].item_button[item_id].prop.on_screen = 0;

                    // $Q: Nhung child control co pos cao hon child control duoc xoa se giam di 1. 
                    for (int i = display[disp_id].item_button[item_id].prop.item_pos + 1; i <= display[disp_id].number_text + display[disp_id].number_button; i++)
                    {
                        int item_id_temp = Disp_return_item_id(disp_id, i);  // $Q: Disp_return_item_id: Ham tra ve id cua item co position i. i la position, ko fai id.           
                        if (display[disp_id].index_button >= item_id_temp)
                        {
                            if (display[disp_id].item_button[item_id_temp].prop.item_pos == i)
                                display[disp_id].item_button[item_id_temp].prop.item_pos = i - 1;
                        }
                        if (display[disp_id].index_text >= item_id_temp)
                        {
                            if (display[disp_id].item_textbox[item_id_temp].prop.item_pos == i)
                                display[disp_id].item_textbox[item_id_temp].prop.item_pos = i - 1;
                        }

                    }
                    display[disp_id].item_button[item_id].prop.item_pos = 0;
                }
                else if (display[disp_id].type == TYPE_TEXT)
                {
                    int item_id = int.Parse(Txt_ID.Text);
                    ctrl = (Control)display[disp_id].item_textbox[item_id].prop.item_disp;
                    GroupboxList[disp_id].Controls.Remove(ctrl);
                    display[disp_id].item_textbox[item_id].prop.on_screen = 0;

                    // $Q: Nhung child control co pos cao hon child control duoc xoa se giam di 1. 
                    for (int i = display[disp_id].item_textbox[item_id].prop.item_pos + 1; i <= display[disp_id].number_text + display[disp_id].number_button; i++)
                    {
                        int item_id_temp = Disp_return_item_id(disp_id, i);  // $Q: Disp_return_item_id: Ham tra ve id cua item co position i. i la position, ko fai id.           
                        if (display[disp_id].index_button >= item_id_temp)
                        {
                            if (display[disp_id].item_button[item_id_temp].prop.item_pos == i)
                                display[disp_id].item_button[item_id_temp].prop.item_pos = i - 1;
                        }
                        if (display[disp_id].index_text >= item_id_temp)
                        {
                            if (display[disp_id].item_textbox[item_id_temp].prop.item_pos == i)
                                display[disp_id].item_textbox[item_id_temp].prop.item_pos = i - 1;
                        }
                    }
                    display[disp_id].item_textbox[item_id].prop.item_pos = 0;
                }
            }


        }

        private void disp_clear()
        {
            //int count = 0;
            for (int i = 1; i <= number_display; i++)
            {
                //GroupboxList[i] = (Panel)display[i].disp;
                this.Controls.Remove(GroupboxList[i]);
                /* display[i].disp = null;
                 display[i].ctrl_preview_no = null;
                 display[i].ctrl_preview_no = new int[200];
                 GroupboxList[i] = null;
                 display[i].disp = null;
                 count++;*/
            }
            Array.Clear(GroupboxList, 0, number_display + 1); // $Q: Co the thay bang Array.Clear(GroupboxList, 1, number_display)
            Array.Resize(ref GroupboxList, 2);
        }

        private void disp_preview(int disp_id)
        {
            // t?o form m?i
            //Btn_Preview.Enabled = false;
            display_no_preview = disp_id;// So sanh display_no_preview d? disable c當 n偀 d?i vs display dang preview
            flag_preview = true;
            display_no_preview = disp_id;// display_no_preview d? x當 d?nh display_no n瀰 dang preview d? disable c當 n偀.
            prv_form = new Form();
            Panel tmppnl;
            int SCREEN_HEIGHT = Screen.PrimaryScreen.Bounds.Height;
            int SCREEN_WIDTH = Screen.PrimaryScreen.Bounds.Width;
            int TASKBAR_HEIGHT = 40;
            prv_form.StartPosition = FormStartPosition.CenterScreen;
            prv_form.AutoScroll = true;
            prv_form.FormClosed += new FormClosedEventHandler(disp_prv_form_closed);
            // t?o m?t groupbox m?i
            tmppnl = disp_clone_panel(disp_id);
            tmppnl.Location = new Point(10, 10);
            prv_form.Width = (tmppnl.Width <= SCREEN_WIDTH - 50) ?
                                tmppnl.Width + 50 :
                                SCREEN_WIDTH;     // c? d?nh size
            //prv_form.Width = tmpgrpb.Width + 20;
            prv_form.Height = (tmppnl.Height <= SCREEN_HEIGHT - TASKBAR_HEIGHT - 70) ?
                                tmppnl.Height + 70 :
                                SCREEN_HEIGHT - TASKBAR_HEIGHT;
            //prv_form.MaximumSize = new Size(prv_form.Width, prv_form.Height);
            //prv_form.MinimumSize = new Size(prv_form.Width, prv_form.Height);
            //prv_form.MaximizeBox = false;
            // hi?n groupbox m?i
            prv_form.Controls.Add(tmppnl);
            prv_form.Text = /*"Display" + */display[disp_id].name/*disp_id.ToString()*/ + " (" + tmppnl.Width + "x" + tmppnl.Height + ")";
            prv_form.Show();
            display[disp_id].Allow_drag = false;

            //Disable c當 n偀

            Btn_Display.Enabled = false; Menu_Edit_create_display.Enabled = false;
            Btn_IMPORT.Enabled = false; Menu_File_load.Enabled = false;
            Btn_Button.Enabled = false; Menu_Edit_add_button.Enabled = false;
            Btn_Text.Enabled = false; Menu_Edit_add_text.Enabled = false;
            Btn_Export.Enabled = false; Menu_File_save.Enabled = false;
            Btn_Preview.Enabled = false; Menu_View_preview.Enabled = false;
            Btn_Clear.Enabled = false; Menu_Edit_clear.Enabled = false;
            Btn_Delete.Enabled = false; Menu_Edit_delete_item.Enabled = false;
            Btn_Copy.Enabled = false; Menu_Edit_copy.Enabled = false;
            Btn_Paste.Enabled = false; Menu_Edit_paste.Enabled = false;
            Btn_Set.Enabled = false;
            Menu_View_English.Enabled = false; Menu_View_Japanese.Enabled = false;
            prop_disable_all();
        }

        private void disp_scale()
        {
            int i;
            int item_id;
            int index_btn = 0;
            int index_txt = 0;
            Button btn;
            Label txt;
            //GroupBox grp;
            GroupboxList[display_no] = (Panel)display[display_no].disp;
            disp_calculate_ratio();
           
            for (i = 1; i <= display[display_no].number_button; i++)
            {
                item_id = index_btn + i;
                btn = (Button)display[display_no].item_button[item_id].prop.item_disp;
                if (display[display_no].item_button[item_id].prop.use == 1)
                {
                    GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio); // SCALE UP
                    btn.Size = new Size(display[display_no].item_button[item_id].prop.size_width, display[display_no].item_button[item_id].prop.size_height);
                    btn.Location = new Point(display[display_no].item_button[item_id].prop.location_x, display[display_no].item_button[item_id].prop.location_y);
                    GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio_recip); // SCALE DOWN
                    if (display[display_no].item_button[item_id].prop.image_off != null)
                    {
                        try
                        {
                            Image img = Image.FromFile(@display[display_no].item_button[item_id].prop.image_off);
                            Bitmap bmp = new Bitmap(img, btn.Width, btn.Height);
                            btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                        }
                        catch { }
                    }
                }
                else
                {
                    index_btn++;
                    i--;
                }
            }
            for (i = 1; i <= display[display_no].number_text; i++)
            {
                item_id = i + index_txt;
                txt = (Label)display[display_no].item_textbox[item_id].prop.item_disp;
                if (display[display_no].item_textbox[item_id].prop.use == 1)
                {
                    GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio); // SCALE UP
                    txt.Size = new Size(display[display_no].item_textbox[item_id].prop.size_width, display[display_no].item_textbox[item_id].prop.size_height);
                    txt.Location = new Point(display[display_no].item_textbox[item_id].prop.location_x, display[display_no].item_textbox[item_id].prop.location_y);
                    GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio_recip); // SCALE DOWN
                }
                else
                {
                    index_txt++;
                    i--;
                }
            }
        }

        private void disp_prv_form_closed(object sender, FormClosedEventArgs e)
        {
            flag_preview = false;
            Btn_Preview.Enabled = true;
            display[display_no_preview].Allow_drag = true;
            display_no_preview = 0;
            ///////
            Btn_Copy.Enabled = true; Menu_Edit_copy.Enabled = true;
            Btn_Paste.Enabled = true; Menu_Edit_paste.Enabled = true;
            Btn_Delete.Enabled = true; Menu_Edit_delete_item.Enabled = true;
            Btn_Clear.Enabled = true; Menu_Edit_clear.Enabled = true;
            Btn_Export.Enabled = true; Menu_File_save.Enabled = true;
            Btn_Preview.Enabled = true; Menu_View_preview.Enabled = true;
            Btn_Button.Enabled = true; Menu_Edit_add_button.Enabled = true;
            Btn_Text.Enabled = true; Menu_Edit_add_text.Enabled = true;
            Menu_View_English.Enabled = true; Menu_View_Japanese.Enabled = true;
            Btn_Display.Enabled = true; Menu_Edit_create_display.Enabled = true;
            Btn_Set.Enabled = true;
            Btn_IMPORT.Enabled = true; Menu_File_load.Enabled = true;
            /////////////
            prop_display(type_before_preview);
        }

        private void disp_show_current_display()
        {
            if (old_disp_id <= number_display)
            {
                if (display[old_disp_id].disp != null || old_disp_id == 0)
                {
                    if (old_disp_id != 0)
                        GroupboxList[old_disp_id].Visible = false;
                    GroupboxList[display_no].Visible = true;

                }
            }
            else
            {
                GroupboxList[display_no].Visible = true;
            }
        }

        /// <summary>
        /// Tr? v? 1 panel m?i d? preview
        /// </summary>
        /// <param name="display_id">display id d? tr? v?</param>
        /// <returns>panel m?i</returns>
        private Panel disp_clone_panel(int display_id)
        {
            Panel oldgrb = (Panel)display[display_id].disp;
            Panel newgrb = new Panel();
            int item_id = 0;

            newgrb.MouseClick += new MouseEventHandler(Disp_Disp_Mouse_Click);
            newgrb.Location = new Point(0, 0);
            newgrb.Width = display[display_id].size_width;
            newgrb.Height = display[display_id].size_height;
            newgrb.BackColor = Color.White;
            newgrb.Name = "panel" + display_id;

            newgrb.Visible = true;
            newgrb.Name = "groupbox" + display_no;

            //clone each control
            // duy?t qua c當 control dt?o

           // Console.WriteLine("display[display_id].item_check[item_id] : " + display[display_id].item_check[item_id]);
            for (int i = 1; i <= Pos_Txt.Maximum; i++)
            {
                item_id = Disp_return_item_id(display_id, i);
                if (display[display_id].index_button >= item_id)
                {
                    if (display[display_id].item_button[item_id].prop.item_pos == i)
                    {
                        Control newctrl;
                        //oldctrl = (Control)display[display_id].item[ctrl_id].prop.item_disp;
                        newctrl = disp_clone_ctrl(display_id, item_id, TYPE_BUTTON);
                        newgrb.Controls.Add(newctrl);
                        newctrl.BringToFront();
                    }
                }
                if (display[display_id].index_text >= item_id)
                {
                    if (display[display_id].item_textbox[item_id].prop.item_pos == i)
                    {

                        Control newctrl;
                        //oldctrl = (Control)display[display_id].item[ctrl_id].prop.item_disp;
                        newctrl = disp_clone_ctrl(display_id, item_id, TYPE_TEXT);
                        newgrb.Controls.Add(newctrl);
                        newctrl.BringToFront();
                    }
                }
            }
                
                return newgrb;
        }

        /// <summary>
        /// nh穗 b?n m?t control m?i
        /// </summary>
        /// <param name="display_id"></param>
        /// <param name="oldctrl"></param>
        /// <param name="ctrl_id"></param>
        private Control disp_clone_ctrl(int display_id, int ctrl_id, int type)
        {
            Control oldctrl;
            if (type == TYPE_BUTTON)
            {
                oldctrl = (Control)display[display_id].item_button[ctrl_id].prop.item_disp;
                Button newbtn = new Button();
                Button oldbtn = (Button)oldctrl;
                newbtn.Name = "button" + ctrl_id + " display" + display_id;
                // Change image
                newbtn.MouseDown += Disp_ctrl_Mouse_Down;
                newbtn.MouseUp += Disp_ctrl_Mouse_Up;
                newbtn.MouseMove += Disp_ctrl_Mouse_Move;
                newbtn.Width = display[display_id].item_button[ctrl_id].prop.size_width;
                newbtn.Height = display[display_id].item_button[ctrl_id].prop.size_height;
                newbtn.Location = new Point(display[display_id].item_button[ctrl_id].prop.location_x,
                                            display[display_id].item_button[ctrl_id].prop.location_y);
                newbtn.Text = oldctrl.Text;

                if (display[display_id].item_button[ctrl_id].prop.size_text > 0)
                {
                    newbtn.Font = new Font("Arial", display[display_id].item_button[ctrl_id].prop.size_text);
                }// text size 

                if (display[display_id].item_button[ctrl_id].prop.is_toggle_button)
                {
                    if (display[display_id].item_button[ctrl_id].prop.button_state == TOGGLE_BUTTON_STATE.OFF)
                    {
                        if (display[display_id].item_button[ctrl_id].prop.image_off != null)
                        {
                            newbtn.Image = disp_get_img_from_filename(display[display_id].item_button[ctrl_id].prop.image_off, newbtn.Width, newbtn.Height);
                        }
                    }
                    else if (display[display_id].item_button[ctrl_id].prop.button_state == TOGGLE_BUTTON_STATE.ON)
                    {
                        if (display[display_id].item_button[ctrl_id].prop.image_on != null)
                        {
                            newbtn.Image = disp_get_img_from_filename(display[display_id].item_button[ctrl_id].prop.image_on, newbtn.Width, newbtn.Height);
                        }
                    }
                }// is_toggle_button
                else
                {
                    if (display[display_id].item_button[ctrl_id].prop.image_off != null)
                    {
                        newbtn.Image = disp_get_img_from_filename(display[display_id].item_button[ctrl_id].prop.image_off, newbtn.Width, newbtn.Height);
                    }
                }
                //newbtn.Image = oldbtn.Image;
                //newbtn.ForeColor = Color.FromArgb(display[display_id].item[ctrl_id].prop.font_color_off_r,
                //                                  display[display_id].item[ctrl_id].prop.font_color_off_g,
                //                                  display[display_id].item[ctrl_id].prop.font_color_off_b);
                newbtn.ForeColor = oldbtn.ForeColor;

                return newbtn;
            }
            else if (type == TYPE_TEXT)
            {
                oldctrl = (Control)display[display_id].item_textbox[ctrl_id].prop.item_disp;
                Label newtxt = new Label();
                Label oldtxt = (Label)oldctrl;
                //  newtxt.Paint += new PaintEventHandler(Disp_Txt_Paint);
                newtxt.MouseDown += Disp_ctrl_Mouse_Down;
                newtxt.MouseUp += Disp_ctrl_Mouse_Up;
                newtxt.MouseMove += Disp_ctrl_Mouse_Move;
                //newtxt.Multiline = true;
                newtxt.Name = "text" + ctrl_id + " display" + display_id;
                // newtxt.ReadOnly = true;
                newtxt.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                newtxt.Cursor = Cursors.Default;
                //if (display[display_id].item[i].prop.align_text == "TopCenter")
                //{
                //    newtxt.TextAlign = ContentAlignment.TopCenter;
                //}
                //else if (display[display_id].item[i].prop.align_text == "TopLeft")
                //{
                //    newtxt.TextAlign = ContentAlignment.TopLeft;
                //}
                //else if (display[display_id].item[i].prop.align_text == "TopRight")
                //{
                //    newtxt.TextAlign = ContentAlignment.TopRight;
                //}
                //else if (display[display_id].item[i].prop.align_text == "BottomCenter")
                //{
                //    newtxt.TextAlign = ContentAlignment.BottomCenter;
                //}
                //else if (display[display_id].item[i].prop.align_text == "BottomLeft")
                //{
                //    newtxt.TextAlign = ContentAlignment.BottomLeft;
                //}
                //else if (display[display_id].item[i].prop.align_text == "BottomRight")
                //{
                //    newtxt.TextAlign = ContentAlignment.BottomRight;
                //}
                //else if (display[display_id].item[i].prop.align_text == "MiddleCenter")
                //{
                //    newtxt.TextAlign = ContentAlignment.MiddleCenter;
                //}
                //else if (display[display_id].item[i].prop.align_text == "MiddleLeft")
                //{
                //    newtxt.TextAlign = ContentAlignment.MiddleLeft;
                //}
                //else if (display[display_id].item[i].prop.align_text == "MiddleRight")
                //{
                //    newtxt.TextAlign = ContentAlignment.MiddleRight;
                //}
                newtxt.TextAlign = oldtxt.TextAlign;
                //newtxt.TextAlign = ContentAlignment.TopCenter;
                newtxt.Width = display[display_id].item_textbox[ctrl_id].prop.size_width;
                newtxt.Height = display[display_id].item_textbox[ctrl_id].prop.size_height;
                newtxt.Location = new Point(display[display_id].item_textbox[ctrl_id].prop.location_x, display[display_id].item_textbox[ctrl_id].prop.location_y);
                newtxt.BackColor = Color.FromArgb(display[display_id].item_textbox[ctrl_id].prop.color_r,
                                                display[display_id].item_textbox[ctrl_id].prop.color_g,
                                                display[display_no].item_textbox[ctrl_id].prop.color_b);
                newtxt.ForeColor = Color.FromArgb(display[display_id].item_textbox[ctrl_id].prop.font_color_off_r,
                                                display[display_id].item_textbox[ctrl_id].prop.font_color_off_g,
                                                display[display_no].item_textbox[ctrl_id].prop.font_color_off_b);
                if (display[display_id].item_textbox[ctrl_id].prop.size_text > 0)
                {
                    newtxt.Font = new Font("Arial", display[display_id].item_textbox[ctrl_id].prop.size_text);
                }
                newtxt.Text = oldtxt.Text;
                newtxt.BorderStyle = oldtxt.BorderStyle;
                newtxt.Tag = oldtxt.Tag;
                return newtxt;
            }
            // copy sang control m?i vadd v瀰 groupbox m?i

            else
                return null;
        }

        /// <summary>
        /// Get an Image object for displaying
        /// </summary>
        /// <param name="filename">filename to retrieve</param>
        /// <param name="width">width of image</param>
        /// <param name="height">height of image</param>
        /// <returns>Image object</returns>
        private Image disp_get_img_from_filename(string filename, int width, int height)
        {
            try
            {
                Image img = Image.FromFile(@filename);
                Bitmap bmp = new Bitmap(img, width, height);
                img = Image.FromHbitmap(bmp.GetHbitmap());
                return img;
            }
            catch
            {
                return null;
            }
        }

        private void disp_change_language(LANG lang)
        {
            string temp;
            int id_temp = 0;
            TreeNode Selected = null;
            int item_id;
            int index_button = 0;
            int index_text = 0;

            // Chuy?n ng ng? s? d?ng
            // duy?t vchuy?n d?i ng ng?
            for (int d = 1; d <= number_display; d++)
            {
                index_button = 0;
                index_text = 0;
                treeView1.SelectedNode = display[d].Button_tree.FirstNode;
                if (treeView1.SelectedNode != null)
                {
                    for (int i = 1; i <= display[d].number_button; i++)
                    {
                        item_id = i + index_button;
                        if (display[d].item_button[item_id].prop.use != 0)
                        {
                            Selected = treeView1.SelectedNode;
                            Match match = Regex.Match(Selected.Text, @"(\d+)");

                            if (match.Success)
                            {
                                temp = (match.Groups[1].Value);
                                id_temp = int.Parse(temp);
                            }
                            if (treeView1.SelectedNode != null)
                            {
                                if (treeView1.SelectedNode.Text.Substring(0, 7 + id_temp.ToString().Length) == "button " + item_id)
                                {

                                    if (Menu_View_English.Checked == true)
                                    {
                                        treeView1.SelectedNode.Text = "button " + item_id + " " + display[d].item_button[item_id].prop.text_eng;
                                    }
                                    else
                                    {
                                        treeView1.SelectedNode.Text = "button " + item_id + " " + display[d].item_button[item_id].prop.text_jpn;
                                    }
                                    treeView1.SelectedNode = treeView1.SelectedNode.NextNode;
                                }
                            }
                            if (display[d].item_button[item_id].prop.on_screen != 0)
                            {
                                Control ctrl = (Control)display[d].item_button[item_id].prop.item_disp;
                                ctrl.Text = (lang == LANG.ENGLISH) ? display[d].item_button[item_id].prop.text_eng :
                                                                             display[d].item_button[item_id].prop.text_jpn;
                            }
                        }
                        else
                        {
                            i--;
                            index_button++;
                        }
                    }
                }
                treeView1.SelectedNode = display[d].Text_tree.FirstNode;
                if (treeView1.SelectedNode != null)
                {
                    for (int i = 1; i <= display[d].number_text; i++)
                    {
                        item_id = i + index_text;
                        if (display[d].item_textbox[item_id].prop.use != 0)
                        {

                            if (treeView1.SelectedNode != null)
                            {
                                Selected = treeView1.SelectedNode;
                                Match match = Regex.Match(Selected.Text, @"(\d+)");

                                if (match.Success)
                                {
                                    temp = (match.Groups[1].Value);
                                    id_temp = int.Parse(temp);
                                }
                                if (treeView1.SelectedNode.Text.Substring(0, 8 + id_temp.ToString().Length) == "textbox " + item_id)
                                {
                                    if (Menu_View_English.Checked == true)
                                    {
                                        treeView1.SelectedNode.Text = "textbox " + item_id + " " + display[d].item_textbox[item_id].prop.text_eng;
                                    }
                                    else
                                    {
                                        treeView1.SelectedNode.Text = "textbox " + item_id + " " + display[d].item_textbox[item_id].prop.text_jpn;
                                    }
                                    treeView1.SelectedNode = treeView1.SelectedNode.NextNode;
                                    if (treeView1.SelectedNode == null)
                                        treeView1.SelectedNode = display[d].Text_tree.LastNode;
                                }
                            }
                            if (display[d].item_textbox[item_id].prop.on_screen != 0)
                            {
                                Control ctrl = (Control)display[d].item_textbox[item_id].prop.item_disp;
                                ctrl.Text = (lang == LANG.ENGLISH) ? display[d].item_textbox[item_id].prop.text_eng :
                                                                             display[d].item_textbox[item_id].prop.text_jpn;
                            }
                        }
                        else
                        {
                            i--;
                            index_text++;
                        }
                    }
                }
            }
        }



        /// <summary>
        /// Ts疣g ctrl du?c ch?n b麩 TreeView
        /// </summary>
        /// <param name="ctrl_id">item id c?n ts疣g trong display_no</param>
        private void disp_select_ctrl(int disp_id, int ctrl_id, int type)
        {
            if (type == TYPE_BUTTON)
            {
                Control ctrl = (Control)display[disp_id].item_button[ctrl_id].prop.item_disp;
                disp_normal_state();
                // Tag property contain real backcolor
                selected_ctrl = ctrl;
                //timer1.Start();
                disp_highlight_ctrl();
            }
            else if (type == TYPE_TEXT)
            {
                Control ctrl = (Control)display[disp_id].item_textbox[ctrl_id].prop.item_disp;
                disp_normal_state();
                // Tag property contain real backcolor
                selected_ctrl = ctrl;
                //timer1.Start();
                disp_highlight_ctrl();
            }

        }

        private void disp_highlight_ctrl()
        {
            Button btn;
            Label lbl;


            if (selected_ctrl != null)
            {
                if (selected_ctrl is Button)
                {
                    btn = (Button)selected_ctrl;

                    btn.FlatStyle = FlatStyle.Flat;
                    btn.FlatAppearance.BorderColor = Color.LightBlue;
                    btn.FlatAppearance.BorderSize = 2;
                    //btn.FlatStyle = FlatStyle.Standard;
                }
                else if (selected_ctrl is Label)
                {
                    lbl = (Label)selected_ctrl;

                    //if (display[display_no].item[Disp_return_id_no(selected_ctrl)].prop.is_label)
                    //{
                    //    lbl.BorderStyle = BorderStyle.FixedSingle;
                    //}
                    //else
                    //{
                    //    lbl.BorderStyle = BorderStyle.None;
                    //}
                    lbl.BackColor = Color.LightBlue;
                }
                //if(flag_preview == false)
                //selected_ctrl.BringToFront();
            }
        }

        private void disp_normal_state()
        {
            Button btn;
            Label lbl;

            if (selected_ctrl is Button)
            {
                btn = (Button)selected_ctrl;
                btn.FlatStyle = FlatStyle.Standard;
            }
            else if (selected_ctrl is Label)
            {
                int ctrl_id;
                lbl = (Label)selected_ctrl;

                ctrl_id = Disp_return_id_no(selected_ctrl);
                //lbl.BackColor = Color.FromArgb(display[display_no].item[ctrl_id].prop.color_r, display[display_no].item[ctrl_id].prop.color_g, display[display_no].item[ctrl_id].prop.color_b);
                lbl.BackColor = (Color)lbl.Tag;
                // label
                //if (display[disp_id].item[ctrl_id].prop.is_label)
                //{
                //    lbl.BorderStyle = BorderStyle.None;
                //}
                //else
                //{
                //    lbl.BorderStyle = BorderStyle.FixedSingle;
                //}

            }
        }

        #endregion

        /*******************FUNCTION PROPERTIES**********************/
        #region FUNCTION PROPERTIES
        private void properties(int disp_id, int type, int id)
        {
            /*if (id /100 == 0)
            {
                if (Txt_Width.Text == "" || Txt_Width.Text == "")
                {
                    MessageBox.Show("Missing information");
                   
                }
                else
                {

                    display[display_no].type = TYPE_DISPLAY;
                    Txt_ID.Text = index_display.ToString();
                    display[display_no].item[id].prop.size_width = int.Parse(Txt_Height.Text);
                    display[display_no].item[id].prop.size_height = int.Parse(Txt_Width.Text);
                }
            }*/
            //if ((id / 100 == 1) || (id / 100 == 2))
            if (action == CREATE)
            {
                if (type == TYPE_BUTTON)
                    display[disp_id].item_button[id].prop.item_pos = display[disp_id].number_text + display[disp_id].number_button;
                else if (type == TYPE_TEXT)
                    display[disp_id].item_textbox[id].prop.item_pos = display[disp_id].number_text + display[disp_id].number_button;

            }
            else
            {
                if (type == TYPE_BUTTON)
                    display[disp_id].item_button[id].prop.item_pos = Decimal.ToInt32(Pos_Txt.Value);
                else if (type == TYPE_TEXT)
                    display[disp_id].item_textbox[id].prop.item_pos = Decimal.ToInt32(Pos_Txt.Value);
            }


            if (type == TYPE_BUTTON)
            {
                display[disp_id].type = TYPE_BUTTON;
                Txt_ID.Text = id.ToString();
                try
                {
                    if ((int.Parse(Txt_X.Text) + int.Parse(Txt_Width.Text)) <= display[display_no].size_width)
                        display[disp_id].item_button[id].prop.location_x = int.Parse(Txt_X.Text);
                }
                catch { }
                try
                {
                    if ((int.Parse(Txt_Y.Text) + int.Parse(Txt_Height.Text)) <= display[display_no].size_height)
                        display[disp_id].item_button[id].prop.location_y = int.Parse(Txt_Y.Text.ToString());
                }
                catch { }
                try
                {
                    if (int.Parse(Txt_Width.Text) <= display[display_no].size_width && int.Parse(Txt_Width.Text) > 0)
                        display[disp_id].item_button[id].prop.size_width = int.Parse(Txt_Width.Text);
                }
                catch { }
                try
                {
                    if (int.Parse(Txt_Height.Text) <= display[display_no].size_height && int.Parse(Txt_Height.Text) > 0)
                        display[disp_id].item_button[id].prop.size_height = int.Parse(Txt_Height.Text);
                }
                catch { }
                display[disp_id].item_button[id].prop.image_off = Txt_Image_Off.Text;
                display[disp_id].item_button[id].prop.image_press = Txt_Image_Press.Text;
                try
                {
                    display[disp_id].item_button[id].prop.size_text = float.Parse(Txt_Size_Text.Text);
                }
                catch { }
                display[disp_id].item_button[id].prop.font_color_off_r = int.Parse(text_color_off_r);
                display[disp_id].item_button[id].prop.font_color_off_g = int.Parse(text_color_off_g);
                display[disp_id].item_button[id].prop.font_color_off_b = int.Parse(text_color_off_b);
                display[disp_id].item_button[id].prop.font_color_press_r = int.Parse(text_color_press_r);
                display[disp_id].item_button[id].prop.font_color_press_g = int.Parse(text_color_press_g);
                display[disp_id].item_button[id].prop.font_color_press_b = int.Parse(text_color_press_b);
                //display[disp_id].item[id].prop.color_r = int.Parse(Txt_Back_Color_R.Text);
                //display[disp_id].item[id].prop.color_g = int.Parse(Txt_Back_Color_G.Text);
                //display[disp_id].item[id].prop.color_b = int.Parse(Txt_Back_Color_B.Text);
                display[disp_id].item_button[id].prop.text_jpn = Txt_Jpn.Text;
                display[disp_id].item_button[id].prop.text_eng = Txt_Eng.Text;

                display[disp_id].item_button[id].prop.is_toggle_button = Chk_Btn_State_On.Checked;
                //n?u ltoggle btn m?i cimage_on, text_color_on
                if (display[disp_id].item_button[id].prop.is_toggle_button)
                {
                    display[disp_id].item_button[id].prop.image_on = Txt_Image_On.Text;
                    display[disp_id].item_button[id].prop.font_color_on_r = int.Parse(text_color_on_r);
                    display[disp_id].item_button[id].prop.font_color_on_g = int.Parse(text_color_on_g);
                    display[disp_id].item_button[id].prop.font_color_on_b = int.Parse(text_color_on_b);
                }
                // }

            }
            if (type == TYPE_TEXT)
            {
                //if (Txt_Width.Text == "" || Txt_Height.Text == "" || Txt_X.Text == "" || Txt_Y.Text == "" || Txt_Size_Text.Text == "")
                //{
                //    MessageBox.Show("Missing information");

                //}
                //if (int.Parse(Txt_Width.Text) > display[disp_id].size_width || int.Parse(Txt_Height.Text) > display[disp_id].size_height || (int.Parse(Txt_X.Text) + int.Parse(Txt_Width.Text)) > display[disp_id].size_width || (int.Parse(Txt_Y.Text) + int.Parse(Txt_Height.Text)) > display[disp_id].size_height || int.Parse(Txt_Width.Text) < 1 || int.Parse(Txt_Height.Text) < 1)
                //{
                //    MessageBox.Show("Refill size");
                //}

                //else
                //{

                display[display_no].type = TYPE_TEXT;
                Txt_ID.Text = id.ToString();
                try
                {
                    if ((int.Parse(Txt_X.Text) + int.Parse(Txt_Width.Text)) <= display[display_no].size_width)
                        display[disp_id].item_textbox[id].prop.location_x = int.Parse(Txt_X.Text);
                }
                catch { }
                try
                {
                    if ((int.Parse(Txt_Y.Text) + int.Parse(Txt_Height.Text)) <= display[display_no].size_height)
                        display[disp_id].item_textbox[id].prop.location_y = int.Parse(Txt_Y.Text.ToString());
                }
                catch { }
                try
                {
                    if (int.Parse(Txt_Width.Text) <= display[display_no].size_width)
                        display[disp_id].item_textbox[id].prop.size_width = int.Parse(Txt_Width.Text);
                }
                catch { }
                try
                {
                    if (int.Parse(Txt_Height.Text) <= display[display_no].size_height)
                        display[disp_id].item_textbox[id].prop.size_height = int.Parse(Txt_Height.Text);
                }
                catch { }
                display[disp_id].item_textbox[id].prop.text_jpn = Txt_Jpn.Text;
                display[disp_id].item_textbox[id].prop.text_eng = Txt_Eng.Text;
                try
                {
                    display[disp_id].item_textbox[id].prop.size_text = float.Parse(Txt_Size_Text.Text);
                }
                catch { }
                display[disp_id].item_textbox[id].prop.font_color_off_r = int.Parse(text_color_off_r);
                display[disp_id].item_textbox[id].prop.font_color_off_g = int.Parse(text_color_off_g);
                display[disp_id].item_textbox[id].prop.font_color_off_b = int.Parse(text_color_off_b);
                display[disp_id].item_textbox[id].prop.color_r = int.Parse(back_color_r);
                display[disp_id].item_textbox[id].prop.color_g = int.Parse(back_color_g);
                display[disp_id].item_textbox[id].prop.color_b = int.Parse(back_color_b);
                display[disp_id].item_textbox[id].prop.align_text = Com_Text_Align.Text;
                display[disp_id].item_textbox[id].prop.is_label = Chk_Lab.Checked;
                //display[disp_id].item[id].prop.led = Chk_Led.Checked;
                //if (display[disp_id].item[id].prop.led)
                //{
                //    display[disp_id].item[id].prop.led_on_color = Com_Led_On.Text;
                //    display[disp_id].item[id].prop.led_off_color = Com_Led_Off.Text;
                //}
            }
            //}
        }

        private void prop_disable_all()
        {
            Txt_X.Enabled = false;
            Txt_Y.Enabled = false;
            Txt_Width.Enabled = false;
            Txt_Height.Enabled = false;
            Txt_Jpn.Enabled = false;
            Txt_Eng.Enabled = false;
            Txt_Size_Text.Enabled = false;
            Txt_Copy.Enabled = false;
            Com_Text_Align.Enabled = false;
            Chk_Btn_State_On.Enabled = false;
            Btn_Back_Color.Enabled = false;
            Btn_Text_Color_On.Enabled = false;
            Btn_Text_Color_Off.Enabled = false;
            Btn_Text_Color_Press.Enabled = false;
            Btn_Open_Image_On.Enabled = false;
            Btn_Open_Image_Off.Enabled = false;
            Btn_Open_Image_Press.Enabled = false;
            Txt_Image_On.Enabled = false;
            Txt_Image_Off.Enabled = false;
            Txt_Image_Press.Enabled = false;
        }
        /// <summary>
        /// H?u hi?u/vhi?u c當 khung nh?p properties t theo lo?i item dang add/edit
        /// </summary>
        /// <param name="id">X當 d?nh ki?u c?a item d?a v瀰 s? id</param>
        private void prop_display(int id)
        {
            if (id == -1)
            {
                Lab_ID.Enabled = false;
                Lab_axis.Enabled = false;
                Lab_Axis_X.Enabled = false;
                Lab_Axis_Y.Enabled = false;
                Lab_Size.Enabled = false;
                Lab_Size_Width.Enabled = false;
                Lab_Size_Height.Enabled = false;
                Lab_Text_Jpn.Enabled = false;
                Lab_Text_Eng.Enabled = false;
                Lab_Size_Text.Enabled = false;
                Lab_Back_Color.Enabled = false;
                Lab_Text_Color.Enabled = false;
                Lab_Image_On.Enabled = false;
                Lab_Image_Press.Enabled = false;
                Lab_Image_Off.Enabled = false;
                Lab_Btn_State_On.Enabled = false;
                Lab_Lab.Enabled = false;
                Lab_Pos.Enabled = false;

                Txt_ID.Enabled = false;
                Txt_X.Enabled = false;
                Txt_Y.Enabled = false;
                Txt_Width.Enabled = false;
                Txt_Height.Enabled = false;
                Txt_Jpn.Enabled = false;
                Txt_Eng.Enabled = false;
                Txt_Size_Text.Enabled = false;
                Pos_Txt.Enabled = false;

                Txt_Image_On.Enabled = false;
                Txt_Image_Off.Enabled = false;
                Txt_Image_Press.Enabled = false;
                Btn_Open_Image_On.Enabled = false;
                Btn_Open_Image_Off.Enabled = false;
                Btn_Open_Image_Press.Enabled = false;
                Btn_Back_Color.Enabled = false;
                Btn_Text_Color_On.Enabled = false;
                Btn_Text_Color_Off.Enabled = false;
                Btn_Text_Color_Press.Enabled = false;

                /*Lab_Led.Enabled = false;
                Chk_Led.Enabled = false;
                Lab_Led_On.Enabled = false;
                Lab_Led_Off.Enabled = false;
                Com_Led_Off.Enabled = false;
                Com_Led_On.Enabled = false;*/
                Lab_Text_Align.Enabled = false;
                Com_Text_Align.Enabled = false;

                Chk_Btn_State_On.Enabled = false;
                Chk_Lab.Enabled = false;
                Pos_Txt.ResetText();
                //Lab_display.Enabled = false;
                Txt_display.Enabled = false;
            }
            if (id == 0)
            {
                Lab_ID.Enabled = false;
                Lab_axis.Enabled = false;
                Lab_Axis_X.Enabled = false;
                Lab_Axis_Y.Enabled = false;
                Lab_Size.Enabled = false;
                Lab_Size_Width.Enabled = false;
                Lab_Size_Height.Enabled = false;
                Lab_Text_Jpn.Enabled = false;
                Lab_Text_Eng.Enabled = false;
                Lab_Size_Text.Enabled = false;
                Lab_Text_Color.Enabled = false;
                Lab_Back_Color.Enabled = false;
                Lab_Image_On.Enabled = false;
                Lab_Image_Press.Enabled = false;
                Lab_Image_Off.Enabled = false;
                Lab_Btn_State_On.Enabled = false;

                Txt_ID.Enabled = false;
                Txt_X.Enabled = false;
                Txt_Y.Enabled = false;
                Txt_Width.Enabled = false;
                Txt_Height.Enabled = false;
                Txt_Jpn.Enabled = false;
                Txt_Eng.Enabled = false;
                Txt_Size_Text.Enabled = false;


                Txt_Image_On.Enabled = false;
                Txt_Image_Off.Enabled = false;
                Txt_Image_Press.Enabled = false;
                Btn_Open_Image_On.Enabled = false;
                Btn_Open_Image_Off.Enabled = false;
                Btn_Open_Image_Press.Enabled = false;
                /*Com_Led_On.Enabled = false;
                Com_Led_Off.Enabled = false;
                Lab_Led_On.Enabled = false;
                Lab_Led_Off.Enabled = false;*/
                Btn_Back_Color.Enabled = false;
                Btn_Text_Color_On.Enabled = false;
                Btn_Text_Color_Off.Enabled = false;
                Btn_Text_Color_Press.Enabled = false;
                Chk_Btn_State_On.Enabled = false;

                Txt_Width.Focus();
            }
            if (1 <= id && id < 99)
            {
                Lab_ID.Enabled = false;
                Lab_axis.Enabled = false;
                Lab_Axis_X.Enabled = false;
                Lab_Axis_Y.Enabled = false;
                Lab_Size.Enabled = true;
                Lab_Size_Width.Enabled = true;
                Lab_Size_Height.Enabled = true;
                Lab_Text_Jpn.Enabled = false;
                Lab_Text_Eng.Enabled = false;
                Lab_Size_Text.Enabled = false;
                Lab_Text_Color.Enabled = false;
                Lab_Back_Color.Enabled = false;
                Lab_Image_On.Enabled = false;
                Lab_Image_Press.Enabled = false;
                Lab_Image_Off.Enabled = false;
                Lab_Btn_State_On.Enabled = false;
                Lab_Pos.Enabled = false;

                Txt_ID.Enabled = false;
                Txt_X.Enabled = false;
                Txt_Y.Enabled = false;
                Txt_Width.Enabled = true;
                Txt_Height.Enabled = true;
                Txt_Jpn.Enabled = false;
                Txt_Eng.Enabled = false;
                Txt_Size_Text.Enabled = false;
                Pos_Txt.Enabled = false;

                Txt_Image_On.Enabled = false;
                Txt_Image_Off.Enabled = false;
                Txt_Image_Press.Enabled = false;
                Btn_Open_Image_On.Enabled = false;
                Btn_Open_Image_Off.Enabled = false;
                Btn_Open_Image_Press.Enabled = false;
                Btn_Back_Color.Enabled = false;
                Btn_Text_Color_On.Enabled = false;
                Btn_Text_Color_Off.Enabled = false;
                Btn_Text_Color_Press.Enabled = false;

                /*Chk_Led.Enabled = false;
                Lab_Led.Enabled = false;*/
                Lab_Text_Align.Enabled = false;
                Com_Text_Align.Enabled = false;
                /* Com_Led_On.Enabled = false;
                 Com_Led_Off.Enabled = false;
                 Lab_Led_On.Enabled = false;
                 Lab_Led_Off.Enabled = false;*/
                Chk_Btn_State_On.Enabled = false;

                Txt_Width.Focus();
            }
            if (id == TYPE_BUTTON)
            {
                Lab_ID.Enabled = false;
                Txt_ID.Enabled = false;
                Lab_axis.Enabled = true;
                Lab_Axis_X.Enabled = true;
                Lab_Axis_Y.Enabled = true;
                Lab_Size.Enabled = true;
                Lab_Size_Width.Enabled = true;
                Lab_Size_Height.Enabled = true;
                Lab_Text_Jpn.Enabled = true;
                Lab_Text_Eng.Enabled = true;
                Lab_Size_Text.Enabled = true;
                Lab_Text_Color.Enabled = true;
                Lab_Back_Color.Enabled = false;
                Lab_Image_Press.Enabled = true;
                Lab_Image_Off.Enabled = true;
                Lab_Pos.Enabled = true;

                Txt_X.Enabled = true;
                Txt_Y.Enabled = true;
                Txt_Width.Enabled = true;
                Txt_Height.Enabled = true;
                Txt_Jpn.Enabled = true;
                Txt_Eng.Enabled = true;
                Txt_Size_Text.Enabled = true;
                Txt_Image_Off.Enabled = true;
                Txt_Image_Press.Enabled = true;
                Btn_Open_Image_Off.Enabled = true;
                Btn_Open_Image_Press.Enabled = true;
                Btn_Back_Color.Enabled = false;
                Btn_Text_Color_Off.Enabled = true;
                Btn_Text_Color_Press.Enabled = true;
                /*Chk_Led.Enabled = false;
                Lab_Led.Enabled = false;*/
                Lab_Text_Align.Enabled = false;
                Com_Text_Align.Enabled = false;
                /*Com_Led_On.Enabled = false;
                Com_Led_Off.Enabled = false;
                Lab_Led_On.Enabled = false;
                Lab_Led_Off.Enabled = false;*/
                Lab_Btn_State_On.Enabled = true; Chk_Btn_State_On.Enabled = true;

                if (Chk_Btn_State_On.Checked == true)
                {
                    Btn_Text_Color_On.Enabled = true;
                    Txt_Image_On.Enabled = true;
                    Btn_Open_Image_On.Enabled = true;
                    Lab_Image_On.Enabled = true;
                }
                else
                {
                    Txt_Image_On.Enabled = false;
                    Btn_Text_Color_On.Enabled = false;
                    Btn_Open_Image_On.Enabled = false;
                    Lab_Image_On.Enabled = false;
                }


                Lab_Lab.Enabled = false;
                Chk_Lab.Enabled = false;
                Pos_Txt.Enabled = true;

                Txt_X.Focus();
            }
            if (id == TYPE_TEXT)
            {
                Lab_ID.Enabled = false;
                Txt_ID.Enabled = false;
                Lab_Image_On.Enabled = false;
                Lab_Image_Off.Enabled = false;
                Lab_Image_Press.Enabled = false;
                Txt_Image_On.Enabled = false;
                Txt_Image_Off.Enabled = false;
                Txt_Image_Press.Enabled = false;
                Btn_Open_Image_On.Enabled = false;
                Btn_Open_Image_Press.Enabled = false;
                Btn_Open_Image_Off.Enabled = false;
                Lab_axis.Enabled = true;
                Lab_Axis_X.Enabled = true;
                Lab_Axis_Y.Enabled = true;
                Lab_Size.Enabled = true;
                Lab_Size_Width.Enabled = true;
                Lab_Size_Height.Enabled = true;
                Lab_Text_Jpn.Enabled = true;
                Lab_Text_Eng.Enabled = true;
                Lab_Size_Text.Enabled = true;
                Lab_Back_Color.Enabled = true;
                Lab_Text_Color.Enabled = true;
                Lab_Btn_State_On.Enabled = false;
                Lab_Pos.Enabled = true;

                Txt_X.Enabled = true;
                Txt_Y.Enabled = true;
                Txt_Width.Enabled = true;
                Txt_Height.Enabled = true;
                Txt_Jpn.Enabled = true;
                Txt_Eng.Enabled = true;
                Txt_Size_Text.Enabled = true;

                Btn_Back_Color.Enabled = true;
                Btn_Text_Color_On.Enabled = false;
                Btn_Text_Color_Off.Enabled = true;
                Btn_Text_Color_Press.Enabled = false;
                /*Lab_Led.Enabled = true;
                Chk_Led.Enabled = true;*/
                Lab_Text_Align.Enabled = true;
                Com_Text_Align.Enabled = true;
                Chk_Btn_State_On.Enabled = false;
                Lab_Lab.Enabled = true;
                Chk_Lab.Enabled = true;
                Pos_Txt.Enabled = true;

                Txt_X.Focus();
            }
        }

        /// <summary>
        /// ﾐi?n c當 gitr? hi?n t?i c?a item dang du?c ch?n
        /// </summary>
        /// <param name="disp_id">display id d? thao t當</param>
        /// <param name="id">item du?c ch?n</param>
        private void prop_display_edit(int disp_id, int type, object ctrl)
        {

            prop_enable_disable_onchange_trigger(true);
            /*if (id /100== 0)
            {
                prop_reset();
                prop_display(id);
                Txt_ID.Text = id.ToString() ;
                Txt_Width.Text = display[id].item[id].prop.size_width.ToString();
                Txt_Height.Text = display[id].item[id].prop.size_height.ToString();
            }*/
            //Txt_display.Text = disp_id.ToString();
            if (type == TYPE_BUTTON)
            {
                int id = Disp_return_id_no(ctrl);
                Txt_ID.Text = id.ToString();
                Txt_X.Text = display[disp_id].item_button[id].prop.location_x.ToString();
                Txt_Y.Text = display[disp_id].item_button[id].prop.location_y.ToString();
                Txt_Width.Text = display[disp_id].item_button[id].prop.size_width.ToString();
                Txt_Height.Text = display[disp_id].item_button[id].prop.size_height.ToString();
                Txt_Image_Off.Text = display[disp_id].item_button[id].prop.image_off;
                Txt_Image_Press.Text = display[disp_id].item_button[id].prop.image_press;
                Txt_Size_Text.Text = display[disp_id].item_button[id].prop.size_text.ToString();
                Txt_Jpn.Text = display[disp_id].item_button[id].prop.text_jpn;
                Txt_Eng.Text = display[disp_id].item_button[id].prop.text_eng;
                Pos_Txt.Value = display[disp_id].item_button[id].prop.item_pos;
                if (Pos_Txt.Value == 1)
                {
                    if (Pos_Txt.Maximum == 1)
                    {
                        flag_pos_max_change = true;
                        Pos_Txt.Maximum = 2;
                        flag_pos_max_change = false;
                        Pos_Txt.Value = Pos_Txt.Value + 1;
                        Pos_Txt.Value = Pos_Txt.Value - 1;
                        Pos_Txt.Maximum = 1;
                    }
                    else
                    {
                        Pos_Txt.Value = Pos_Txt.Value + 1;
                        Pos_Txt.Value = Pos_Txt.Value - 1;
                    }

                }
                else if (Pos_Txt.Maximum != 1)
                {
                    Pos_Txt.Value = Pos_Txt.Value - 1;
                    Pos_Txt.Value = Pos_Txt.Value + 1;
                }

                //button_state_check = display[disp_id].item[id].prop.button_state;
                text_color_off_r = display[disp_id].item_button[id].prop.font_color_off_r.ToString();
                text_color_off_g = display[disp_id].item_button[id].prop.font_color_off_g.ToString();
                text_color_off_b = display[disp_id].item_button[id].prop.font_color_off_b.ToString();
                Btn_Text_Color_Off.BackColor = Color.FromArgb(int.Parse(text_color_off_r), int.Parse(text_color_off_g), int.Parse(text_color_off_b));
                if (int.Parse(text_color_off_r) > 127 && int.Parse(text_color_off_g) > 127 && int.Parse(text_color_off_b) > 127)
                    Btn_Text_Color_Off.ForeColor = Color.FromArgb(0, 0, 0);
                else
                    Btn_Text_Color_Off.ForeColor = Color.FromArgb(255, 255, 255);
                Btn_Back_Color.BackColor = Color.FromName("White");

                text_color_press_r = display[disp_id].item_button[id].prop.font_color_press_r.ToString();
                text_color_press_g = display[disp_id].item_button[id].prop.font_color_press_g.ToString();
                text_color_press_b = display[disp_id].item_button[id].prop.font_color_press_b.ToString();
                Btn_Text_Color_Press.BackColor = Color.FromArgb(int.Parse(text_color_press_r), int.Parse(text_color_press_g), int.Parse(text_color_press_b));
                if (int.Parse(text_color_press_r) > 127 && int.Parse(text_color_press_g) > 127 && int.Parse(text_color_press_b) > 127)
                    Btn_Text_Color_Press.ForeColor = Color.FromArgb(0, 0, 0);
                else
                    Btn_Text_Color_Press.ForeColor = Color.FromArgb(255, 255, 255);
                Com_Text_Align.ResetText();
                Chk_Btn_State_On.Checked = display[disp_id].item_button[id].prop.is_toggle_button;
                Txt_Image_On.Clear();
                if (display[disp_id].item_button[id].prop.is_toggle_button)
                {
                    Lab_Image_On.Enabled = true;
                    Txt_Image_On.Enabled = true;
                    Btn_Open_Image_On.Enabled = true;
                    Btn_Text_Color_On.Enabled = true;
                    Txt_Image_On.Text = display[disp_id].item_button[id].prop.image_on;
                    text_color_on_r = display[disp_id].item_button[id].prop.font_color_on_r.ToString();
                    text_color_on_g = display[disp_id].item_button[id].prop.font_color_on_g.ToString();
                    text_color_on_b = display[disp_id].item_button[id].prop.font_color_on_b.ToString();
                    Btn_Text_Color_On.BackColor = Color.FromArgb(int.Parse(text_color_on_r), int.Parse(text_color_on_g), int.Parse(text_color_on_b));
                    if (int.Parse(text_color_on_r) > 127 && int.Parse(text_color_on_g) > 127 && int.Parse(text_color_on_b) > 127)
                        Btn_Text_Color_On.ForeColor = Color.FromArgb(0, 0, 0);
                    else
                        Btn_Text_Color_On.ForeColor = Color.FromArgb(255, 255, 255);
                }
                prop_display(TYPE_BUTTON);
            }
            else if (type == TYPE_TEXT)
            {
                Chk_Btn_State_On.Checked = false;
                int id = Disp_return_id_no(ctrl);
                Txt_ID.Text = id.ToString();
                Txt_X.Text = display[disp_id].item_textbox[id].prop.location_x.ToString();
                Txt_Y.Text = display[disp_id].item_textbox[id].prop.location_y.ToString();
                Txt_Width.Text = display[disp_id].item_textbox[id].prop.size_width.ToString();
                Txt_Height.Text = display[disp_id].item_textbox[id].prop.size_height.ToString();
                Txt_Size_Text.Text = display[disp_id].item_textbox[id].prop.size_text.ToString();
                Txt_Jpn.Text = display[disp_id].item_textbox[id].prop.text_jpn;
                Txt_Eng.Text = display[disp_id].item_textbox[id].prop.text_eng;
                text_color_off_r = display[disp_id].item_textbox[id].prop.font_color_off_r.ToString();
                text_color_off_g = display[disp_id].item_textbox[id].prop.font_color_off_g.ToString();
                text_color_off_b = display[disp_id].item_textbox[id].prop.font_color_off_b.ToString();
                Btn_Text_Color_Off.BackColor = Color.FromArgb(int.Parse(text_color_off_r), int.Parse(text_color_off_g), int.Parse(text_color_off_b));
                if (int.Parse(text_color_off_r) > 127 && int.Parse(text_color_off_g) > 127 && int.Parse(text_color_off_b) > 127)
                    Btn_Text_Color_Off.ForeColor = Color.FromArgb(0, 0, 0);
                else
                    Btn_Text_Color_Off.ForeColor = Color.FromArgb(255, 255, 255);
                back_color_r = display[disp_id].item_textbox[id].prop.color_r.ToString();
                back_color_g = display[disp_id].item_textbox[id].prop.color_g.ToString();
                back_color_b = display[disp_id].item_textbox[id].prop.color_b.ToString();
                Btn_Back_Color.BackColor = Color.FromArgb(int.Parse(back_color_r), int.Parse(back_color_g), int.Parse(back_color_b));
                if (int.Parse(back_color_r) > 127 && int.Parse(back_color_g) > 127 && int.Parse(back_color_b) > 127)
                    Btn_Back_Color.ForeColor = Color.FromArgb(0, 0, 0);
                else
                    Btn_Back_Color.ForeColor = Color.FromArgb(255, 255, 255);
                Com_Text_Align.Text = display[disp_id].item_textbox[id].prop.align_text.ToString();
                Pos_Txt.Value = display[disp_id].item_textbox[id].prop.item_pos;
                if (Pos_Txt.Value == 1)
                {
                    if (Pos_Txt.Maximum == 1)
                    {
                        flag_pos_max_change = true;
                        Pos_Txt.Maximum = 2;
                        flag_pos_max_change = false;
                        Pos_Txt.Value = Pos_Txt.Value + 1;
                        Pos_Txt.Value = Pos_Txt.Value - 1;
                        Pos_Txt.Maximum = 1;
                    }
                    else
                    {
                        Pos_Txt.Value = Pos_Txt.Value + 1;
                        Pos_Txt.Value = Pos_Txt.Value - 1;
                    }

                }
                else if (Pos_Txt.Maximum != 1)
                {
                    Pos_Txt.Value = Pos_Txt.Value - 1;
                    Pos_Txt.Value = Pos_Txt.Value + 1;
                }
                //Thang them
                Txt_Image_On.Clear();
                Txt_Image_Off.Clear();
                Txt_Image_Press.Clear();
                // label
                Chk_Lab.Checked = display[disp_id].item_textbox[id].prop.is_label;
                prop_display(TYPE_TEXT);
                //led
                //Chk_Led.Checked = display[disp_id].item[id].prop.led;
                //if (display[disp_id].item[id].prop.led)
                //{
                //    Com_Led_On.Enabled = true;
                //    Com_Led_Off.Enabled = true;
                //    Com_Led_On.Text = display[disp_id].item[id].prop.led_on_color;
                //    Com_Led_Off.Text = display[disp_id].item[id].prop.led_off_color;
                //}
                //else {
                //    Com_Led_Off.SelectedIndex = -1;
                //    Com_Led_On.SelectedIndex = -1;
                //}
            }
            if (flag_preview == true && display_no == display_no_preview)
            {
                prv_form.BringToFront();
            }

            prop_enable_disable_onchange_trigger(false);

        }

        /// <summary>
        /// X ho?c cho ph駱 c當 h瀘 x? l s? ki?n textchanged tr麩 c當 Txt properties
        /// </summary>
        /// <param name="disabling">Vhi?u c當 h瀘 x? l event</param>
        private void prop_enable_disable_onchange_trigger(bool disabling)
        {
            if (disabling == true)
            {

                Txt_X.TextChanged -= Txt_props_Changed;
                Txt_Y.TextChanged -= Txt_props_Changed;
                Txt_Width.TextChanged -= Txt_props_Changed;
                Txt_Height.TextChanged -= Txt_props_Changed;
                Txt_Jpn.TextChanged -= Txt_props_Changed;
                Txt_Eng.TextChanged -= Txt_props_Changed;
                //Txt_Text_Color_B.TextChanged -= Txt_props_Changed;
                //Txt_Text_Color_G.TextChanged -= Txt_props_Changed;
                //Txt_Text_Color_R.TextChanged -= Txt_props_Changed;
                //Txt_Back_Color_B.TextChanged -= Txt_props_Changed;
                //Txt_Back_Color_G.TextChanged -= Txt_props_Changed;
                //Txt_Back_Color_R.TextChanged -= Txt_props_Changed;
                Txt_Size_Text.TextChanged -= Txt_props_Changed;
                Txt_Image_On.TextChanged -= Txt_props_Changed;
                Txt_Image_Off.TextChanged -= Txt_props_Changed;
                Txt_Image_Press.TextChanged -= Txt_props_Changed;
                Btn_Text_Color_Press.BackColorChanged -= Txt_props_Changed;
                Btn_Text_Color_Off.BackColorChanged -= Txt_props_Changed;
                Btn_Text_Color_On.BackColorChanged -= Txt_props_Changed;
                Btn_Back_Color.BackColorChanged -= Txt_props_Changed;
                Com_Text_Align.TextChanged -= Txt_props_Changed;
                Txt_Size_Text.TextChanged -= Txt_props_Changed;
                //Chk_Lab.CheckedChanged -= Chk_Lab_CheckedChanged;
            }
            else
            {
                Txt_X.TextChanged += Txt_props_Changed;
                Txt_Y.TextChanged += Txt_props_Changed;
                Txt_Width.TextChanged += Txt_props_Changed;
                Txt_Height.TextChanged += Txt_props_Changed;
                Txt_Jpn.TextChanged += Txt_props_Changed;
                Txt_Eng.TextChanged += Txt_props_Changed;
                //Txt_Text_Color_B.TextChanged += Txt_props_Changed;
                //Txt_Text_Color_G.TextChanged += Txt_props_Changed;
                //Txt_Text_Color_R.TextChanged += Txt_props_Changed;
                //Txt_Back_Color_B.TextChanged += Txt_props_Changed;
                //Txt_Back_Color_G.TextChanged += Txt_props_Changed;
                //Txt_Back_Color_R.TextChanged += Txt_props_Changed;
                Txt_Size_Text.TextChanged += Txt_props_Changed;
                Txt_Image_On.TextChanged += Txt_props_Changed;
                Txt_Image_Off.TextChanged += Txt_props_Changed;
                Txt_Image_Press.TextChanged += Txt_props_Changed;
                Btn_Text_Color_Press.BackColorChanged += Txt_props_Changed;
                Btn_Text_Color_Off.BackColorChanged += Txt_props_Changed;
                Btn_Text_Color_On.BackColorChanged += Txt_props_Changed;
                Btn_Back_Color.BackColorChanged += Txt_props_Changed;
                Com_Text_Align.TextChanged += Txt_props_Changed;
                Txt_Size_Text.TextChanged += Txt_props_Changed;
                //Chk_Lab.CheckedChanged += Chk_Lab_CheckedChanged;
            }
        }

        /// <summary>
        /// X tr?ng c當 khung nh?p properties
        /// </summary>
        private void prop_reset()
        {
            prop_enable_disable_onchange_trigger(false);
            //  Txt_display.Clear();
            Txt_ID.Clear();
            Txt_X.Clear();
            Txt_Y.Clear();
            Txt_Width.Clear();
            Txt_Height.Clear();
            Txt_Image_On.Clear();
            Txt_Image_Off.Clear();
            Txt_Image_Press.Clear();
            //Txt_Copy.Clear();

            Txt_Size_Text.ResetText();
            Txt_Jpn.Clear();
            Txt_Eng.Clear();
            Pos_Txt.ResetText();
            Btn_Back_Color.Enabled = false;
            Btn_Text_Color_On.Enabled = false;
            Btn_Back_Color.BackColor = Color.FromName("White");
            Btn_Text_Color_On.BackColor = Color.FromName("White");
            Btn_Text_Color_Off.BackColor = Color.FromName("White");
            Btn_Text_Color_Press.BackColor = Color.FromName("White");
            Com_Text_Align.ResetText();
            /*Chk_Led.Checked = false;
            Chk_Led.Enabled = false;
            Lab_Led.Enabled = false;*/
            Lab_Text_Align.Enabled = false;
            Com_Text_Align.Enabled = false;
            Chk_Btn_State_On.Checked = false;
            prop_enable_disable_onchange_trigger(true); //$Q: lua chon viec co them hay bo su kien Txt_props_Changed
            /* Com_Led_Off.SelectedIndex = -1;
             Com_Led_On.SelectedIndex = -1;*/
        }

        /// <summary>
        /// ﾐi?n c當 gitr? m?c d?nh v瀰 c當 khung nh?p properties
        /// </summary>
        /// <param name="id">X當 d?nh ki?u c?a item d?a v瀰 s? id</param>
        private void prop_default(int id)
        {
            if (id == TYPE_DISPLAY)
            {
                Txt_Width.Text = "640";
                Txt_Height.Text = "480";
            }
            if (id == TYPE_BUTTON)
            {
                Txt_X.Text = "0";
                Txt_Y.Text = "0";
                Txt_Width.Text = "120";
                Txt_Height.Text = "80";
                Txt_Size_Text.Text = "12";
                Txt_Eng.Text = "Button";
                Txt_Jpn.Text = "ボタン";
                Btn_Text_Color_Off.BackColor = Color.FromName("Black");
                Btn_Text_Color_Off.ForeColor = Color.FromName("White");
                text_color_on_r = "128";
                text_color_on_g = "255";
                text_color_on_b = "128";
                text_color_off_r = "0";
                text_color_off_g = "0";
                text_color_off_b = "0";
                text_color_press_r = "255";
                text_color_press_g = "255";
                text_color_press_b = "255";

            }
            if (id == TYPE_TEXT)
            {
                Txt_X.Text = "0";
                Txt_Y.Text = "0";
                Txt_Width.Text = "120";
                Txt_Height.Text = "80";
                Txt_Size_Text.Text = "12";
                Txt_Eng.Text = "Text";
                Txt_Jpn.Text = "テキスト";
                Btn_Back_Color.BackColor = Color.FromName("White");

                Btn_Text_Color_Off.BackColor = Color.FromName("Black");
                Btn_Text_Color_Off.ForeColor = Color.FromName("White");
                Com_Text_Align.Text = "TopCenter";

                text_color_off_r = "0";
                text_color_off_g = "0";
                text_color_off_b = "0";

                back_color_r = "255";
                back_color_g = "255";
                back_color_b = "255";
            }
        }

        private void prop_disp_prop(int id)
        {
            if (Txt_Width.Text == "" || Txt_Height.Text == "")
            {
                MessageBox.Show("Missing information");

            }
            else if ((int.Parse(Txt_Height.Text) > 5000) || (int.Parse(Txt_Width.Text) > 5000) || (int.Parse(Txt_Width.Text) < 1) || (int.Parse(Txt_Height.Text) < 1))
            {
                MessageBox.Show("Width and Height from 1 to 5000!!!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            else
            {
                int count_button = 0;
                int count_text = 0;
                flag_display_edit = 1;
                /* Kiem tra xem Height hoac Width cua Display sau khi edit co nho hon tat ca cac button tinh tu y va x cua button*/
                /* Trong truong hop neu Display chua co button nao co the bo qua */
                for (int i = 1; count_button != display[id].number_button; i++)
                {
                    if (display[id].item_button[i].prop.use == 1)
                    {
                        int x = display[id].item_button[i].prop.location_x;
                        int y = display[id].item_button[i].prop.location_y;
                        int height = display[id].item_button[i].prop.size_height;
                        int width = display[id].item_button[i].prop.size_width;

                        if (int.Parse(Txt_Height.Text) < y + height || int.Parse(Txt_Width.Text) < x + width)
                        {
                            MessageBox.Show("Width and Height are smaller than button", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                            flag_display_edit = 0;
                            break;
                        }

                        count_button++;
                    }
                }
                //flag_display_edit = 1;
                // $Q: sau vong lap tren, neu flag_display_edit van bang 1 thi size display thoa yeu cau doi voi size cua button. Neu = 0 thi ko can kiem tra child_control text
                for (int i = 1; (count_text != display[id].number_text) && flag_display_edit == 1; i++)
                {
                    if (display[id].item_textbox[i].prop.use == 1)
                    {
                        int x = display[id].item_textbox[i].prop.location_x;
                        int y = display[id].item_textbox[i].prop.location_y;
                        int height = display[id].item_textbox[i].prop.size_height;
                        int width = display[id].item_textbox[i].prop.size_width;

                        if (int.Parse(Txt_Height.Text) < y + height || int.Parse(Txt_Width.Text) < x + width)
                        {
                            MessageBox.Show("Width and Height are smaller than text", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                            flag_display_edit = 0;
                            break;
                        }
                        count_text++;
                    }
                }
                if (flag_display_edit == 1) // $Q: truong hop size Display thoa yeu cau doi voi size cua tat ca child control
                {
                    display[id].type = TYPE_DISPLAY;
                    Txt_display.Text = id.ToString();
                    display[id].size_width = int.Parse(Txt_Width.Text);
                    display[id].size_height = int.Parse(Txt_Height.Text);
                }
            }
        }

        private void prop_disp_disp_edit(int id)
        {
            prop_reset();
            //Pos_Txt.Value=0;
            prop_display(1);
            Txt_display.Text = id.ToString();
            // Txt_ID.Text = display_no.ToString();
            Txt_Width.Text = display[id].size_width.ToString();
            Txt_Height.Text = display[id].size_height.ToString();
            if (flag_preview == true)
            {
                prv_form.BringToFront();
            }
        }

        #endregion

        /*******************FUNCTION EXPORT**************************/
        #region FUNCTION EXPORT
        /*  public void Expo_Xuat()
        {
            int i;
            
            int id;
            int btn_index=100;
            int txt_index = 200;
            SaveFileDialog save = new SaveFileDialog();
            save.Filter = "TXT file(*.txt)|*.txt|HTML file(*.html)|*.html|XML file(*.xml)|*.xml|C Sharp file(*.cs)|*.cs";
            save.ShowDialog();
            if (save.FileName != "")
            {
                FileInfo f = new FileInfo(save.FileName);
                StreamWriter writer = f.CreateText();
                for (i = 1; i <= display[display_no].number_button; i++)
                {
                    if (display[display_no].item[btn_index + i].prop.use == 1)
                    {
                        id = btn_index + i;
                        writer.Write("Type: button");
                        writer.Write("\nID No: " + id);
                        writer.Write("\nHeight: " + display[display_no].item[btn_index + i].prop.size_height);
                        writer.Write("\nWidth: " + display[display_no].item[btn_index + i].prop.size_width);
                        writer.Write("\nX axis: " + display[display_no].item[btn_index + i].prop.location_x);
                        writer.Write("\nY axis: " + display[display_no].item[btn_index + i].prop.location_y);
                        writer.Write("\nSize text: " + display[display_no].item[btn_index + i].prop.size_text);
                        writer.Write("\nText jpn: " + display[display_no].item[btn_index + i].prop.text_jpn);
                        writer.Write("\nText eng: " + display[display_no].item[btn_index + i].prop.text_eng);
                        writer.Write("\nImage on: " + display[display_no].item[btn_index + i].prop.image_on);
                        writer.Write("\nImage off: " + display[display_no].item[btn_index + i].prop.image_off);
                        writer.Write("\nText color R: " + display[display_no].item[btn_index + i].prop.font_color_r);
                        writer.Write("\nText color G: " + display[display_no].item[btn_index + i].prop.font_color_g);
                        writer.Write("\nText color B: " + display[display_no].item[btn_index + i].prop.font_color_b);
                        writer.Write("\n\n");
                    }
                    else
                    {
                        btn_index++;
                        i--;
                    }
                }

                for (i = 1; i <= display[display_no].number_text; i++)
                {
                    if (display[display_no].item[txt_index + i].prop.use == 1)
                    {
                        id = txt_index + i;
                        writer.Write("Type: text");
                        writer.Write("\nID No: " + id);
                        writer.Write("\nHeight: " + display[display_no].item[txt_index + i].prop.size_height);
                        writer.Write("\nWidth: " + display[display_no].item[txt_index + i].prop.size_width);
                        writer.Write("\nX axis: " + display[display_no].item[txt_index + i].prop.location_x);
                        writer.Write("\nY axis: " + display[display_no].item[txt_index + i].prop.location_y);
                        writer.Write("\nSize text: " + display[display_no].item[txt_index + i].prop.size_text);
                        writer.Write("\nText jpn: " + display[display_no].item[txt_index + i].prop.text_jpn);
                        writer.Write("\nText eng: " + display[display_no].item[txt_index + i].prop.text_eng);
                        writer.Write("\nText color R: " + display[display_no].item[txt_index + i].prop.font_color_r);
                        writer.Write("\nText color G: " + display[display_no].item[txt_index + i].prop.font_color_g);
                        writer.Write("\nText color B: " + display[display_no].item[txt_index + i].prop.font_color_b);
                        writer.Write("\nBack color R: " + display[display_no].item[txt_index + i].prop.color_r);
                        writer.Write("\nBack color G: " + display[display_no].item[txt_index + i].prop.color_g);
                        writer.Write("\nBack color B: " + display[display_no].item[txt_index + i].prop.color_b);
                        writer.Write("\n\n");
                    }
                    else
                    {
                        txt_index++;
                        i--;
                    }

                }
                writer.Close();
            }
        }*/




        private int Expo_pos_pre_no(int disp_id, int ctrl_id)
        {
            int pos_pre_no = 0;
            int number_ctr = display[disp_id].number_button + display[disp_id].number_text;
            int i = 199 - number_ctr + 1;
            for (int j = i; j <= 199; j++)
            {
                if (display[disp_id].ctrl_preview_no[j] == ctrl_id)
                {
                    pos_pre_no = number_ctr - (199 - j);
                    break;
                }
            }
            return pos_pre_no;
        }


        public void Expo_Xuat()
        {
            bool flag;
            int string_count;
            int i;
            int j;
            int k;
            int id_disp;
            int id_no;
            int id_item;
            //int disp_index = 0;
            int btn_index = 100;
            int txt_index = 200;
            SaveFileDialog save = new SaveFileDialog();
            save.Title = "Export file";
            save.Filter = "TXT file(*.txt)|*.txt|HTML file(*.html)|*.html|XML file(*.xml)|*.xml|C Sharp file(*.cs)|*.cs";
            if (MessageBox.Show("Do you want to copy image to output's location", "Warning", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                flag = true;
            }
            else
            {
                flag = false;
            }
            save.ShowDialog();
            if (save.FileName != "")
            {
                FileInfo f = new FileInfo(save.FileName);
                string temp = save.FileName.Substring(save.FileName.LastIndexOf("\\") + 1);
                this.Text = temp.Substring(0, temp.Length - 4) + " - LSIVN Hi-Draw - 画面作成ツール";
                if (flag)
                {
                    int count = 0;
                    int count_btn = 0;
                    string sourcefile;
                    string filename;
                    string destinationfile;
                    filename = save.FileName.Substring(save.FileName.LastIndexOf("\\") + 1);
                    this.Text = filename.Substring(0, filename.Length - 4) + " - LSIVN Hi-Draw - 画面作成ツール";
                    destinationfile = save.FileName.Substring(0, save.FileName.Length - filename.Length - 1);
                    destinationfile += "\\Image\\";
                    bool isExists = System.IO.Directory.Exists(destinationfile);
                    if (!isExists)
                        System.IO.Directory.CreateDirectory(destinationfile);
                    for (i = 1; count < number_display; i++)
                    {
                        if (display[i].use == 1)
                        {
                            count_btn = 0;
                            for (j = 1; count_btn < display[i].number_button; j++)
                            {
                                if (display[i].item_button[j].prop.use == 1)
                                {
                                    //Image on
                                    sourcefile = display[i].item_button[j].prop.image_on;
                                    try
                                    {
                                        filename = sourcefile.Substring(sourcefile.LastIndexOf("\\") + 1);
                                        filename = destinationfile + filename;
                                    }
                                    catch { }
                                    if (File.Exists(filename) == false)
                                    {
                                        try
                                        {
                                            File.Delete(filename);
                                            File.Copy(sourcefile, filename);
                                        }
                                        catch { }
                                    }
                                    //Image off
                                    sourcefile = display[i].item_button[j].prop.image_off;
                                    try
                                    {
                                        filename = sourcefile.Substring(sourcefile.LastIndexOf("\\") + 1);
                                        filename = destinationfile + filename;
                                    }
                                    catch { }
                                    if (File.Exists(filename) == false)
                                    {
                                        try
                                        {
                                            File.Delete(filename);
                                            File.Copy(sourcefile, filename);
                                        }
                                        catch { }
                                    }
                                    //Image press
                                    sourcefile = display[i].item_button[j].prop.image_press;
                                    try
                                    {
                                        filename = sourcefile.Substring(sourcefile.LastIndexOf("\\") + 1);
                                        filename = destinationfile + filename;
                                    }
                                    catch { }
                                    if (File.Exists(filename) == false)
                                    {
                                        try
                                        {

                                            File.Delete(filename);
                                            File.Copy(sourcefile, filename);
                                        }
                                        catch { }
                                    }

                                    count_btn++;
                                }

                            }
                            count++;
                        }
                    }
                }
                string display_name;
                StreamWriter writer = f.CreateText();
                id_disp = 0;
                id_no = 0;
                writer.Write("Language:\t");
                if (Menu_View_English.Checked == true)
                    writer.Write("English\r\n");
                else
                    writer.Write("Japanese\r\n");
                for (i = 1; i <= number_display; i++)
                {
                    //id_disp = disp_index + i;
                    btn_index = 0;
                    txt_index = 0;
                    Match match = Regex.Match(display[i].name, @"(\d+)");
                    if (match.Success)
                    {
                        display_name = (match.Groups[1].Value);
                        id_no = int.Parse(display_name);
                        id_disp = list_return_display_no(display[i].name);
                    }
                    // if (display[id_disp].use == 1)
                    // {
                    writer.Write("Type_display");
                    writer.Write("\r\nID_No:\t" + id_no);
                    writer.Write("\r\nHeight:\t" + display[id_disp].size_height);
                    writer.Write("\r\nWidth:\t" + display[id_disp].size_width);
                    writer.Write("\r\n");
                    //}
                    /*  else
                      {
                          i--;
                          disp_index++;
                      }*/

                    for (j = 1; j <= display[id_disp].number_button; j++)
                    {

                        if (display[id_disp].item_button[btn_index + j].prop.use == 1)
                        {
                            id_item = btn_index + j;
                            // $Q     display[id_disp].item_button[id_item].prop.pos_pre_no = Expo_pos_pre_no(id_disp, id_item);
                            writer.Write("\r\n\r\nType_button");
                            writer.Write("\r\nID_No:\t" + id_item);
                            writer.Write("\r\nPosition:\t" + display[id_disp].item_button[id_item].prop.item_pos);
                            writer.Write("\r\nHeight:\t" + display[id_disp].item_button[id_item].prop.size_height);
                            writer.Write("\r\nWidth:\t" + display[id_disp].item_button[id_item].prop.size_width);
                            writer.Write("\r\nX_axis:\t" + display[id_disp].item_button[id_item].prop.location_x);
                            writer.Write("\r\nY_axis:\t" + display[id_disp].item_button[id_item].prop.location_y);
                            writer.Write("\r\nSize_text:\t" + display[id_disp].item_button[id_item].prop.size_text);
                            //writer.Write("\r\nText_jpn:\t\"" + display[id_disp].item[id_item].prop.text_jpn); writer.Write("\"");
                            writer.Write("\r\nText_jpn:\t\"");//+ display[id_disp].item[id_item].prop.text_jpn); writer.Write("\"");
                            for (string_count = 0; string_count < display[id_disp].item_button[id_item].prop.text_jpn.Length; string_count++)
                            {
                                if (display[id_disp].item_button[id_item].prop.text_jpn[string_count] == '\r')
                                {
                                    writer.Write('\\'); writer.Write('r');
                                }
                                else if (display[id_disp].item_button[id_item].prop.text_jpn[string_count] == '\n')
                                {
                                    writer.Write('\\'); writer.Write('n');
                                }
                                else if (display[id_disp].item_button[id_item].prop.text_jpn[string_count] == '\\')
                                {
                                    writer.Write('\\'); writer.Write('\\');
                                }
                                else if (display[id_disp].item_button[id_item].prop.text_jpn[string_count] == '\"')
                                {
                                    writer.Write('\\'); writer.Write('\"');
                                }
                                else
                                    writer.Write(display[id_disp].item_button[id_item].prop.text_jpn[string_count]);
                            }
                            writer.Write("\"");
                            //writer.Write("\r\nText_eng:\t\"" + display[id_disp].item[id_item].prop.text_eng); writer.Write("\"");
                            writer.Write("\r\nText_eng:\t\"");//+ display[id_disp].item[id_item].prop.text_jpn); writer.Write("\"");
                            for (string_count = 0; string_count < display[id_disp].item_button[id_item].prop.text_eng.Length; string_count++)
                            {
                                if (display[id_disp].item_button[id_item].prop.text_eng[string_count] == '\r')
                                {
                                    writer.Write('\\'); writer.Write('r');
                                }
                                else if (display[id_disp].item_button[id_item].prop.text_eng[string_count] == '\n')
                                {
                                    writer.Write('\\'); writer.Write('n');
                                }
                                else if (display[id_disp].item_button[id_item].prop.text_eng[string_count] == '\\')
                                {
                                    writer.Write('\\'); writer.Write('\\');
                                }
                                else if (display[id_disp].item_button[id_item].prop.text_eng[string_count] == '\"')
                                {
                                    writer.Write('\\'); writer.Write('\"');
                                }
                                else
                                    writer.Write(display[id_disp].item_button[id_item].prop.text_eng[string_count]);
                            }
                            writer.Write("\"");
                            if (display[id_disp].item_button[id_item].prop.is_toggle_button == true)
                                writer.Write("\r\nIs_toggle:\t" + 1);
                            else
                                writer.Write("\r\nIs_toggle:\t0");
                            try
                            {
                                if (display[id_disp].item_button[id_item].prop.is_toggle_button == true && display[id_disp].item_button[id_item].prop.image_on != "")
                                { writer.Write("\r\nImage_on:\t\"" + "Image\\" + display[id_disp].item_button[id_item].prop.image_on.Substring(display[id_disp].item_button[id_item].prop.image_on.LastIndexOf("\\") + 1)); writer.Write("\""); }
                                if (display[id_disp].item_button[id_item].prop.image_off != "")
                                { writer.Write("\r\nImage_off:\t\"" + "Image\\" + display[id_disp].item_button[id_item].prop.image_off.Substring(display[id_disp].item_button[id_item].prop.image_off.LastIndexOf("\\") + 1)); writer.Write("\""); }
                                if (display[id_disp].item_button[id_item].prop.image_press != "")
                                { writer.Write("\r\nImage_press:\"" + "Image\\" + display[id_disp].item_button[id_item].prop.image_press.Substring(display[id_disp].item_button[id_item].prop.image_press.LastIndexOf("\\") + 1)); writer.Write("\""); }
                            }
                            catch { }

                            writer.Write("\r\nText_color_off_R:\t" + display[id_disp].item_button[id_item].prop.font_color_off_r);
                            writer.Write("\r\nText_color_off_G:\t" + display[id_disp].item_button[id_item].prop.font_color_off_g);
                            writer.Write("\r\nText_color_off_B:\t" + display[id_disp].item_button[id_item].prop.font_color_off_b);

                            writer.Write("\r\nText_color_on_R:\t" + display[id_disp].item_button[id_item].prop.font_color_on_r);
                            writer.Write("\r\nText_color_on_G:\t" + display[id_disp].item_button[id_item].prop.font_color_on_g);
                            writer.Write("\r\nText_color_on_B:\t" + display[id_disp].item_button[id_item].prop.font_color_on_b);

                            writer.Write("\r\nText_color_press_R:\t" + display[id_disp].item_button[id_item].prop.font_color_press_r);
                            writer.Write("\r\nText_color_press_G:\t" + display[id_disp].item_button[id_item].prop.font_color_press_g);
                            writer.Write("\r\nText_color_press_B:\t" + display[id_disp].item_button[id_item].prop.font_color_press_b);

                            writer.Write("\r\n");
                        }
                        else
                        {
                            btn_index++;
                            j--;
                        }
                    }

                    for (k = 1; k <= display[id_disp].number_text; k++)
                    {
                        if (display[id_disp].item_textbox[txt_index + k].prop.use == 1)
                        {
                            id_item = txt_index + k;
                            // $Q: display[id_disp].item_textbox[id_item].prop.pos_pre_no = Expo_pos_pre_no(id_disp, id_item);
                            writer.Write("\r\n\r\nType_text");
                            writer.Write("\r\nID_No:\t" + id_item);
                            writer.Write("\r\nPosition:\t" + display[id_disp].item_textbox[id_item].prop.item_pos);
                            writer.Write("\r\nHeight:\t" + display[id_disp].item_textbox[id_item].prop.size_height);
                            writer.Write("\r\nWidth:\t" + display[id_disp].item_textbox[id_item].prop.size_width);
                            writer.Write("\r\nX_axis:\t" + display[id_disp].item_textbox[id_item].prop.location_x);
                            writer.Write("\r\nY_axis:\t" + display[id_disp].item_textbox[id_item].prop.location_y);
                            writer.Write("\r\nSize_text:\t" + display[id_disp].item_textbox[id_item].prop.size_text);
                            //writer.Write("\r\nText_jpn:\t\"" + display[id_disp].item[id_item].prop.text_jpn); writer.Write("\"");
                            writer.Write("\r\nText_jpn:\t\"");//+ display[id_disp].item[id_item].prop.text_jpn); writer.Write("\"");
                            for (string_count = 0; string_count < display[id_disp].item_textbox[id_item].prop.text_jpn.Length; string_count++)
                            {
                                if (display[id_disp].item_textbox[id_item].prop.text_jpn[string_count] == '\r')
                                {
                                    writer.Write('\\'); writer.Write('r');
                                }
                                else if (display[id_disp].item_textbox[id_item].prop.text_jpn[string_count] == '\n')
                                {
                                    writer.Write('\\'); writer.Write('n');
                                }
                                else if (display[id_disp].item_textbox[id_item].prop.text_jpn[string_count] == '\\')
                                {
                                    writer.Write('\\'); writer.Write('\\');
                                }
                                else if (display[id_disp].item_textbox[id_item].prop.text_jpn[string_count] == '\"')
                                {
                                    writer.Write('\\'); writer.Write('\"');
                                }
                                else
                                    writer.Write(display[id_disp].item_textbox[id_item].prop.text_jpn[string_count]);
                            }
                            writer.Write("\"");

                            //writer.Write("\r\nText_eng:\t\"" + display[id_disp].item[id_item].prop.text_eng); writer.Write("\"");
                            writer.Write("\r\nText_eng:\t\"");//+ display[id_disp].item[id_item].prop.text_jpn); writer.Write("\"");
                            for (string_count = 0; string_count < display[id_disp].item_textbox[id_item].prop.text_eng.Length; string_count++)
                            {
                                if (display[id_disp].item_textbox[id_item].prop.text_eng[string_count] == '\r')
                                {
                                    writer.Write('\\'); writer.Write('r');
                                }
                                else if (display[id_disp].item_textbox[id_item].prop.text_eng[string_count] == '\n')
                                {
                                    writer.Write('\\'); writer.Write('n');
                                }
                                else if (display[id_disp].item_textbox[id_item].prop.text_eng[string_count] == '\\')
                                {
                                    writer.Write('\\'); writer.Write('\\');
                                }
                                else if (display[id_disp].item_textbox[id_item].prop.text_eng[string_count] == '\"')
                                {
                                    writer.Write('\\'); writer.Write('\"');
                                }
                                else
                                    writer.Write(display[id_disp].item_textbox[id_item].prop.text_eng[string_count]);
                            }
                            writer.Write("\"");
                            if (display[id_disp].item_textbox[id_item].prop.is_label == true)
                                writer.Write("\r\nIs_label:\t" + 1);
                            else
                                writer.Write("\r\nIs_label:\t0");
                            writer.Write("\r\nAlign_text:\t\"" + display[id_disp].item_textbox[id_item].prop.align_text); writer.Write("\"");
                            writer.Write("\r\nText_color_off_R:\t" + display[id_disp].item_textbox[id_item].prop.font_color_off_r);
                            writer.Write("\r\nText_color_off_G:\t" + display[id_disp].item_textbox[id_item].prop.font_color_off_g);
                            writer.Write("\r\nText_color_off_B:\t" + display[id_disp].item_textbox[id_item].prop.font_color_off_b);
                            writer.Write("\r\nBack_color_R:\t\t" + display[id_disp].item_textbox[id_item].prop.color_r);
                            writer.Write("\r\nBack_color_G:\t\t" + display[id_disp].item_textbox[id_item].prop.color_g);
                            writer.Write("\r\nBack_color_B:\t\t" + display[id_disp].item_textbox[id_item].prop.color_b);
                            writer.Write("\r\n");
                        }
                        else
                        {
                            txt_index++;
                            k--;
                        }
                    }
                    writer.Write("-----------------------------------------------------------------\r\n");
                }
                writer.Close();
            }
        }

        public void Expo_Nhap()
        {
            type_import = 0;
            string line;
            int disp_no = 1;
            string destinationfile;
            string filename;
            OpenFileDialog open = new OpenFileDialog();
            open.Filter = "TXT file(*.txt)|*.txt|HTML file(*.html)|*.html|XML file(*.xml)|*.xml|C Sharp file(*.cs)|*.cs";
            open.Title = "Import File";
            open.ShowDialog();
            if (open.FileName != "")
            {
                temp_display = 1;
                for (int i = 1; i <= number_display; i++)
                {
                    Array.Clear(display[i].item_button, 0, display[i].number_button + 1);
                    Array.Resize(ref display[i].item_button, 2);
                    Array.Clear(display[i].item_textbox, 0, display[i].number_text + 1);
                    Array.Resize(ref display[i].item_textbox, 2);
                }

                old_disp_id = 0;
                disp_clear();
                list_clear();

                FileInfo f = new FileInfo(open.FileName);
                filename = open.FileName.Substring(open.FileName.LastIndexOf("\\") + 1);
                destinationfile = open.FileName.Substring(0, open.FileName.Length - filename.Length);
                StreamReader reader = f.OpenText();

                while ((line = reader.ReadLine()) != null)
                {
                    disp_no = Expo_Detailed_Import(line, disp_no, destinationfile);
                }
                reader.Close();
                this.Text = filename.Substring(0, filename.Length - 4) + " - LSIVN Hi-Draw - 画面作成ツール";
            }
            else
            {
                import_fail = 1;
                // MessageBox.Show("File is not selected");

            }
            display[disp_no].id_no = 0;
        }

        public int Expo_Detailed_Import(string input, int disp_no, string input_file_name)
        {
            int i = 0;
            //int j=0;
            int disp_pos = disp_no;
            string temp = "";
            string temp_1 = "";
            string image = input;
            string j = "";
            if (input == "Language:\tJapanese")
                flag_japanese = true;
            else if (input == "Language:\tEnglish")
                flag_japanese = false;
            Match match = Regex.Match(input, @"(\d+)");
            if (match.Success)
            {
                temp = (match.Groups[1].Value);
                try
                {
                    i = int.Parse(temp);
                }
                catch { }
                input = input.Substring(0, input.Length - temp.Length);
            }
            Console.WriteLine("i: " + i);
            if (image.Length > 10)
            {
                temp = image.Substring(11, image.Length - 11);
                if (temp.Length != 0)
                    temp_1 = temp.Substring(0, temp.Length - 1);
                j = image.Substring(0, 10);
                Console.WriteLine("j: " + j);               
            }
            switch (input)
            {
                case "Type_button":
                    type_import = TYPE_BUTTON;
                    display[disp_no].id_count++;
                    break;
                case "Type_text":
                    type_import = TYPE_TEXT;
                    display[disp_no].id_count++;
                    break;
                case "Type_display":
                    type_import = TYPE_DISPLAY;
                    number_display++;
                    temp_display++;
                    Array.Resize(ref display, temp_display);
                    Array.Resize(ref GroupboxList, temp_display);
                    Create_item(number_display);
                    break;
            }
            if (type_import == TYPE_DISPLAY)
            {
                switch (input)
                {
                    case "ID_No:\t":
                        disp_pos = number_display;
                        display[disp_pos].use = 1;
                        display[disp_pos].name = "display " + i;
                        display[disp_pos].type = TYPE_DISPLAY;
                        index_display = i;
                        break;
                    case "Height:\t":
                        display[disp_pos].size_height = i;
                        break;
                    case "Width:\t":
                        display[disp_pos].size_width = i;
                        break;
                }
            }
            else if (type_import == TYPE_BUTTON)
            {
                switch (input)
                {
                    case "ID_No:\t":
                        display[disp_pos].id_no = i;
                        Array.Resize(ref display[disp_pos].id_import_button, display[disp_pos].pos_import_button + 1);
                        display[disp_pos].id_import_button[display[disp_no].pos_import_button] = display[disp_no].id_no;
                        display[disp_pos].index_button = display[disp_no].id_no;
                        Array.Resize(ref display[disp_pos].item_button, display[disp_pos].index_button + 1);
                        display[disp_pos].pos_import_button++;
                        display[disp_pos].item_button[display[disp_no].id_no].prop.use = 1;
                        display[disp_pos].item_button[display[disp_no].id_no].prop.on_screen = 0;
                        display[disp_pos].type = TYPE_BUTTON;
                        break;
                    case "Position:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.item_pos = i;
                        break;
                    case "Height:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.size_height = i;
                        break;
                    case "Width:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.size_width = i;
                        break;
                    case "X_axis:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.location_x = i;
                        break;
                    case "Y_axis:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.location_y = i;
                        break;
                    case "Size_text:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.size_text = i;
                        break;
                    case "Is_toggle:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.is_toggle_button = (i == 1) ? true : false;
                        break;
                    case "Text_color_off_R:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.font_color_off_r = i;
                        break;
                    case "Text_color_off_G:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.font_color_off_g = i;
                        break;
                    case "Text_color_off_B:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.font_color_off_b = i;
                        break;
                    case "Text_color_on_R:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.font_color_on_r = i;
                        break;
                    case "Text_color_on_G:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.font_color_on_g = i;
                        break;
                    case "Text_color_on_B:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.font_color_on_b = i;
                        break;
                    case "Text_color_press_R:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.font_color_press_r = i;
                        break;
                    case "Text_color_press_G:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.font_color_press_g = i;
                        break;
                    case "Text_color_press_B:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.font_color_press_b = i;
                        break;
                    case "Back_color_R:\t\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.color_r = i;
                        break;
                    case "Back_color_G:\t\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.color_g = i;
                        break;
                    case "Back_color_B:\t\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.color_b = i;
                        break;
                }
                switch (j)
                {
                    case "Align_text":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.align_text = temp.Substring(2, temp.Length - 3);
                        break;
                    case "Text_jpn:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.text_jpn = Expo_Return_Init_String(temp_1);
                        //display[disp_pos].item[display[disp_no].id_no].prop.text_jpn = temp_1;
                        break;
                    case "Text_eng:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.text_eng = Expo_Return_Init_String(temp_1);
                        break;
                    case "Image_on:\t":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.image_on = input_file_name + temp_1;
                        break;
                    case "Image_pres":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.image_press = input_file_name + temp_1.Substring(2, temp_1.Length - 2);
                        break;
                    case "Image_off:":
                        display[disp_pos].item_button[display[disp_no].id_no].prop.image_off = input_file_name + temp_1.Substring(1, temp_1.Length - 1);
                        break;
                    /* case "Is_toggle:":
                         display[disp_pos].item[display[disp_no].id_no].prop.is_toggle_button = bool.Parse(temp_1.Substring(1,temp_1.Length-2));
                         break;*/
                }

            }
            else if (type_import == TYPE_TEXT)
            {
                switch (input)
                {
                    case "ID_No:\t":
                        display[disp_pos].id_no = i;
                        Array.Resize(ref display[disp_pos].id_import_textbox, display[disp_pos].pos_import_textbox + 1);
                        display[disp_pos].id_import_textbox[display[disp_no].pos_import_textbox] = display[disp_pos].id_no;
                        display[disp_pos].index_text = display[disp_no].id_no;
                        Array.Resize(ref display[disp_pos].item_textbox, display[disp_pos].index_text + 1);
                        display[disp_pos].pos_import_textbox++;
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.use = 1;
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.on_screen = 0;
                        display[disp_pos].type = TYPE_TEXT;
                        break;
                    case "Position:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.item_pos = i;
                        break;
                    case "Height:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.size_height = i;
                        break;
                    case "Width:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.size_width = i;
                        break;
                    case "X_axis:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.location_x = i;
                        break;
                    case "Y_axis:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.location_y = i;
                        break;
                    case "Size_text:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.size_text = i;
                        break;
                    case "Text_color_off_R:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.font_color_off_r = i;
                        break;
                    case "Text_color_off_G:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.font_color_off_g = i;
                        break;
                    case "Text_color_off_B:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.font_color_off_b = i;
                        break;
                    case "Text_color_on_R:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.font_color_on_r = i;
                        break;
                    case "Text_color_on_G:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.font_color_on_g = i;
                        break;
                    case "Text_color_on_B:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.font_color_on_b = i;
                        break;
                    case "Text_color_press_R:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.font_color_press_r = i;
                        break;
                    case "Text_color_press_G:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.font_color_press_g = i;
                        break;
                    case "Text_color_press_B:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.font_color_press_b = i;
                        break;
                    case "Back_color_R:\t\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.color_r = i;
                        break;
                    case "Back_color_G:\t\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.color_g = i;
                        break;
                    case "Back_color_B:\t\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.color_b = i;
                        break;
                }
                switch (j)
                {
                    case "Align_text":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.align_text = temp.Substring(2, temp.Length - 3);
                        break;
                    case "Text_jpn:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.text_jpn = Expo_Return_Init_String(temp_1);
                        //display[disp_pos].item[display[disp_no].id_no].prop.text_jpn = temp_1;
                        break;
                    case "Text_eng:\t":
                        display[disp_pos].item_textbox[display[disp_no].id_no].prop.text_eng = Expo_Return_Init_String(temp_1);
                        break;
                    case "Is_label:\t":
                        Console.WriteLine("LLLLL");
                        if(i == 1)
                           display[disp_pos].item_textbox[display[disp_no].id_no].prop.is_label = true;
                        break;
                       /* case "Is_toggle:":
                         display[disp_pos].item[display[disp_no].id_no].prop.is_toggle_button = bool.Parse(temp_1.Substring(1,temp_1.Length-2));
                         break;*/
                }
            }
            return disp_pos;
        }

        private void Btn_Open_Image_On_Click(object sender, EventArgs e)
        {
            //flag_is_setting = false;
            string sourcefile;
            string destinationfile = "Image\\";
            string filename;
            OpenFileDialog open = new OpenFileDialog();
            // open.Filter = "TXT file(*.txt)|*.txt|HTML file(*.html)|*.html|XML file(*.xml)|*.xml|C Sharp file(*.cs)|*.cs";
            open.ShowDialog();
            {
                try
                {
                    bool isExists = System.IO.Directory.Exists("Image");
                    if (!isExists)
                        System.IO.Directory.CreateDirectory("Image");
                    sourcefile = open.FileName;
                    filename = open.FileName.Substring(open.FileName.LastIndexOf("\\") + 1); ;
                    destinationfile += filename;
                    if (File.Exists(destinationfile) == false)
                    {
                        File.Delete(destinationfile);
                        File.Copy(sourcefile, destinationfile);

                    }
                    Txt_Image_On.Text = destinationfile;
                }
                catch { }
            }
        }

        private void Btn_Open_Image_Off_Click(object sender, EventArgs e)
        {
            //flag_is_setting = 0;
            string sourcefile;
            string destinationfile = "Image\\";
            string filename;
            OpenFileDialog open = new OpenFileDialog();
            // open.Filter = "TXT file(*.txt)|*.txt|HTML file(*.html)|*.html|XML file(*.xml)|*.xml|C Sharp file(*.cs)|*.cs";
            open.ShowDialog();
            {
                try
                {
                    bool isExists = System.IO.Directory.Exists("Image");
                    if (!isExists)
                        System.IO.Directory.CreateDirectory("Image");
                    sourcefile = open.FileName;
                    filename = open.FileName.Substring(open.FileName.LastIndexOf("\\") + 1); ;
                    destinationfile += filename;
                    if (File.Exists(destinationfile) == false)
                    {
                        File.Delete(destinationfile);
                        File.Copy(sourcefile, destinationfile);
                    }
                    Txt_Image_Off.Text = destinationfile;
                }
                catch { }
            }
        }

        private void Btn_Open_Image_Press_Click(object sender, EventArgs e)
        {
            //flag_is_setting = 0;
            string sourcefile;
            string destinationfile = "Image\\";
            string filename;
            OpenFileDialog open = new OpenFileDialog();
            // open.Filter = "TXT file(*.txt)|*.txt|HTML file(*.html)|*.html|XML file(*.xml)|*.xml|C Sharp file(*.cs)|*.cs";
            open.ShowDialog();
            {
                try
                {
                    bool isExists = System.IO.Directory.Exists("Image");
                    if (!isExists)
                        System.IO.Directory.CreateDirectory("Image");
                    sourcefile = open.FileName;
                    filename = open.FileName.Substring(open.FileName.LastIndexOf("\\") + 1); ;
                    destinationfile += filename;
                    if (File.Exists(destinationfile) == false)
                    {
                        File.Delete(destinationfile);
                        File.Copy(sourcefile, destinationfile);
                    }
                    Txt_Image_Press.Text = destinationfile;
                }
                catch { }
            }
        }
        #endregion


        /************Form Close***************************/
        private void DepartamentEdit_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (modified)
            {
                DialogResult dlg = MessageBox.Show("Save changes?", "Warning", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (dlg == DialogResult.Yes)
                {
                    Expo_Xuat();

                    e.Cancel = false;

                }
                else if (dlg == DialogResult.No)
                {
                    e.Cancel = false;

                }
                else if (dlg == DialogResult.Cancel)
                {
                    e.Cancel = true;
                }
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            //if (ctrl_is_highlighted)
            //{
            //    disp_normal_state();
            //}
            //else
            //{
            //    disp_highlight_ctrl();
            //}
            //ctrl_is_highlighted = !ctrl_is_highlighted;
            disp_normal_state();
            timer1.Stop();
        }

        private void Create_item(int temp)
        {
            display[temp].item_button = new ID_NUMBER[2];
            display[temp].item_textbox = new ID_NUMBER[2];
            display[temp].Allow_drag = true;
            //   display[i].item = new ID_NUMBER[300];
            display[temp].use = 0;
            import_fail = 0;
            display[temp].index_button = 0;
            display[temp].index_text = 0;
            display[temp].id_import_button = new int[1];
            display[temp].id_import_textbox = new int[1];
            display[temp].item_button[1].prop.on_screen = 0;
            display[temp].item_textbox[1].prop.on_screen = 0;
            display[temp].button_delete = new int[100];
            display[temp].text_delete = new int[100];
            display[temp].ctrl_preview_no = new int[200];
            display[temp].number_button = 0;
            display[temp].number_text = 0;
            display[temp].temp_number_button = 0;
            display[temp].temp_number_text = 0;
            display[temp].Button_tree = new TreeNode();
            display[temp].Text_tree = new TreeNode();

        }
        private void Copy_item(int disp_id, int id, ID_NUMBER item_copy, int type)
        {

            if (type == TYPE_BUTTON)
            {
                display[disp_id].type = TYPE_BUTTON;
                Txt_ID.Text = id.ToString();
                display[disp_id].item_button[id].prop.item_pos = item_copy.prop.item_pos;
                display[disp_id].item_button[id].prop.location_x = item_copy.prop.location_x;
                display[disp_id].item_button[id].prop.location_y = item_copy.prop.location_y;
                display[disp_id].item_button[id].prop.size_width = item_copy.prop.size_width;
                display[disp_id].item_button[id].prop.size_height = item_copy.prop.size_height;
                display[disp_id].item_button[id].prop.image_on = item_copy.prop.image_on;
                display[disp_id].item_button[id].prop.image_off = item_copy.prop.image_off;
                display[disp_id].item_button[id].prop.image_press = item_copy.prop.image_press;

                display[disp_id].item_button[id].prop.is_toggle_button = item_copy.prop.is_toggle_button;
                display[disp_id].item_button[id].prop.is_label = item_copy.prop.is_label;
                display[disp_id].item_button[id].prop.size_text = item_copy.prop.size_text;

                display[disp_id].item_button[id].prop.font_color_off_r = item_copy.prop.font_color_off_r;
                display[disp_id].item_button[id].prop.font_color_off_g = item_copy.prop.font_color_off_g;
                display[disp_id].item_button[id].prop.font_color_off_b = item_copy.prop.font_color_off_b;

                display[disp_id].item_button[id].prop.font_color_on_r = item_copy.prop.font_color_on_r;
                display[disp_id].item_button[id].prop.font_color_on_g = item_copy.prop.font_color_on_g;
                display[disp_id].item_button[id].prop.font_color_on_b = item_copy.prop.font_color_on_b;

                display[disp_id].item_button[id].prop.font_color_press_r = item_copy.prop.font_color_press_r;
                display[disp_id].item_button[id].prop.font_color_press_g = item_copy.prop.font_color_press_g;
                display[disp_id].item_button[id].prop.font_color_press_b = item_copy.prop.font_color_press_b;

                display[disp_id].item_button[id].prop.color_b = item_copy.prop.color_b;
                display[disp_id].item_button[id].prop.color_r = item_copy.prop.color_r;
                display[disp_id].item_button[id].prop.color_g = item_copy.prop.color_g;

                display[disp_id].item_button[id].prop.text_jpn = item_copy.prop.text_jpn;
                display[disp_id].item_button[id].prop.text_eng = item_copy.prop.text_eng;
                display[disp_id].item_button[id].prop.use = item_copy.prop.use;
                /* if (Chk_Led.Checked == true)
                 {
                     display[disp_id].item[id].prop.led_on_color = Com_Led_On.Text;
                     display[disp_id].item[id].prop.led_off_color = Com_Led_Off.Text;
                 }*/
            }
            if (type == TYPE_TEXT)
            {
                display[display_no].type = TYPE_TEXT;
                Txt_ID.Text = id.ToString();
                display[disp_id].item_textbox[id].prop.item_pos = item_copy.prop.item_pos;
                display[disp_id].item_textbox[id].prop.location_x = item_copy.prop.location_x;
                display[disp_id].item_textbox[id].prop.location_y = item_copy.prop.location_y;
                display[disp_id].item_textbox[id].prop.size_width = item_copy.prop.size_width;
                display[disp_id].item_textbox[id].prop.size_height = item_copy.prop.size_height;
                display[disp_id].item_textbox[id].prop.text_jpn = item_copy.prop.text_jpn;
                display[disp_id].item_textbox[id].prop.text_eng = item_copy.prop.text_eng;
                display[disp_id].item_textbox[id].prop.size_text = item_copy.prop.size_text;
                display[disp_id].item_textbox[id].prop.font_color_off_r = item_copy.prop.font_color_off_r;
                display[disp_id].item_textbox[id].prop.font_color_off_g = item_copy.prop.font_color_off_g;
                display[disp_id].item_textbox[id].prop.font_color_off_b = item_copy.prop.font_color_off_b;
                display[disp_id].item_textbox[id].prop.color_r = item_copy.prop.color_r;
                display[disp_id].item_textbox[id].prop.color_g = item_copy.prop.color_g;
                display[disp_id].item_textbox[id].prop.color_b = item_copy.prop.color_b;
                display[disp_id].item_textbox[id].prop.align_text = item_copy.prop.align_text;
                display[disp_id].item_textbox[id].prop.use = item_copy.prop.use;
            }
        }
        private void Copy_display(int display_no)
        {
            display[display_no].name = "display " + index_display;
            display[display_no].count_button_delete = display_copy[0].count_button_delete;
            display[display_no].count_text_delete = display_copy[0].count_text_delete;
            //display[display_no].disp = display_copy[0].disp;
            display[display_no].flag_state = display_copy[0].flag_state;
            display[display_no].id_count = display_copy[0].id_count;
            display[display_no].id_import = display_copy[0].id_import;
            display[display_no].id_no = display_copy[0].id_no;
            display[display_no].index_button = display_copy[0].index_button;
            display[display_no].index_text = display_copy[0].index_text;
            display[display_no].limit_button = display_copy[0].limit_button;
            display[display_no].limit_text = display_copy[0].limit_text;
            display[display_no].number_button = display_copy[0].number_button;
            display[display_no].number_text = display_copy[0].number_text;
            display[display_no].pos_import = display_copy[0].pos_import;
            display[display_no].size_height = display_copy[0].size_height;
            display[display_no].size_width = display_copy[0].size_width;
            display[display_no].temp_number_button = display_copy[0].temp_number_button;
            display[display_no].temp_number_text = display_copy[0].temp_number_text;
            display[display_no].text_delete = display_copy[0].text_delete;
            display[display_no].type = display_copy[0].type;
            display[display_no].type_item = display[0].type_item;
            display[display_no].use = display_copy[0].use;
            display[display_no].Allow_drag = display_copy[0].Allow_drag;
            display[display_no].Disp_Scale_ratio = display_copy[0].Disp_Scale_ratio;
            display[display_no].Disp_Scale_ratio_recip = display_copy[0].Disp_Scale_ratio_recip;
            Array.Resize(ref display[display_no].item_button, display[display_no].index_button + 1);
            Array.Resize(ref display[display_no].item_textbox, display[display_no].index_text + 1);
            int index_button = 0;
            int index_text = 0;
            int item_id;
            for (int i = 1; i <= display_copy[0].number_button; i++)
            {

                item_id = i + index_button;

                if (display_copy[0].item_button[item_id].prop.use != 0)
                {
                    Copy_item(display_no, item_id, display_copy[0].item_button[item_id], TYPE_BUTTON);  // $Q: Copy toan bo item cua display dc copy cho display moi
                    display[display_no].item_button[item_id].prop.on_screen = 0;
                }
                else
                {
                    index_button++;
                    i--;
                }
            }

            for (int i = 1; i <= display_copy[0].number_text; i++)
            {
                item_id = i + index_text;
                if (display_copy[0].item_textbox[item_id].prop.use != 0)
                {
                    Copy_item(display_no, item_id, display_copy[0].item_textbox[item_id], TYPE_TEXT);  // $Q: Copy toan bo item cua display dc copy cho display moi
                    display[display_no].item_textbox[item_id].prop.on_screen = 0;
                }
                else
                {
                    index_text++;
                    i--;
                }
            }
            display[display_no].Button_tree = new TreeNode("Button");
            display[display_no].Text_tree = new TreeNode("Text");
            TreeNode[] array = new TreeNode[] { display[display_no].Button_tree, display[display_no].Text_tree };
            display[display_no].Display_tree = new TreeNode(display[display_no].name, array);
            int count_btn = 0;
            int count_txt = 0;
            for (int i = 1; count_btn < display[display_no].number_button; i++)
            {
                if (display[display_no].item_button[i].prop.use == 1)
                {

                    //listBox1.Items.Add("button 10" + display[display_no].index_button);

                    if (Menu_View_English.Checked == true)
                        display[display_no].Button_tree.Nodes.Add("button " + i + " " + display[display_no].item_button[i].prop.text_eng);
                    else
                        display[display_no].Button_tree.Nodes.Add("button " + i + " " + display[display_no].item_button[i].prop.text_jpn);


                    count_btn++;
                }
            }
            for (int i = 1; count_txt < display[display_no].number_text; i++)
            {
                if (display[display_no].item_textbox[i].prop.use == 1)
                {
                    if (Menu_View_English.Checked == true)
                        display[display_no].Text_tree.Nodes.Add("textbox " + i + " " + display[display_no].item_textbox[i].prop.text_eng);
                    else
                        display[display_no].Text_tree.Nodes.Add("textbox " + i + " " + display[display_no].item_textbox[i].prop.text_jpn);
                    count_txt++;
                }
            }
            flag_display_paste = 1;
            //display[display_no].Display_tree = display_copy[0].Display_tree;

        }

        #region Prop_of_prop
        private void Txt_X_KeyPress(object sender, KeyPressEventArgs e)
        {
            Txt_X.MaxLength = 4;
            Txt_Y.MaxLength = 4;
            Txt_Width.MaxLength = 4;
            Txt_Height.MaxLength = 4;

            if (!Char.IsDigit(e.KeyChar) && !Char.IsControl(e.KeyChar))
            {
                e.Handled = true;
            }
        }




        private void Btn_Back_Color_Click(object sender, EventArgs e)
        {

            //thi?t l?p m炒 n?n c0?a textbox1 lm炒 da ch?n
            ColorDialog colorDialog = new ColorDialog();
            if (colorDialog.ShowDialog() == DialogResult.OK)
            {
                back_color_r = colorDialog.Color.R.ToString();
                back_color_g = colorDialog.Color.G.ToString();
                back_color_b = colorDialog.Color.B.ToString();
                Btn_Back_Color.BackColor = colorDialog.Color;
                if (int.Parse(back_color_r) > 127 && int.Parse(back_color_g) > 127 && int.Parse(back_color_b) > 127)
                    Btn_Back_Color.ForeColor = Color.FromArgb(0, 0, 0);
                else
                    Btn_Back_Color.ForeColor = Color.FromArgb(255, 255, 255);
            }
        }

        private void Txt_props_Changed(object sender, EventArgs e)
        {
            int id;
            if (display_no <= number_display)
            {
                if (action == EDIT && display[display_no].type != TYPE_DISPLAY && display_no > 0)
                {
                    //id = Disp_return_id_no();
                    if (Txt_ID.Text != "")
                    {
                        id = int.Parse(Txt_ID.Text);
                        properties(display_no, display[display_no].type, id);
                        if (display[display_no].type == TYPE_BUTTON)
                        {

                            //listBox1.Items.Add("button 10" + display[display_no].index_button);

                            if (Menu_View_English.Checked == true)
                                treeView1.SelectedNode.Text = "button " + id + " " + display[display_no].item_button[id].prop.text_eng;
                            else
                                treeView1.SelectedNode.Text = "button " + id + " " + display[display_no].item_button[id].prop.text_jpn;


                        }
                        else if (display[display_no].type == TYPE_TEXT)
                        {

                            //listBox1.Items.Add("button 10" + display[display_no].index_button);

                            if (Menu_View_English.Checked == true)
                                treeView1.SelectedNode.Text = "textbox " + id + " " + display[display_no].item_textbox[id].prop.text_eng;
                            else
                                treeView1.SelectedNode.Text = "textbox " + id + " " + display[display_no].item_textbox[id].prop.text_jpn;


                        }
                        disp_ctrl_create_edit(display_no, id, display[display_no].type);
                        modified = true;
                    }

                }
            }
        }

        private void Txt_Size_Text_KeyPress_1(object sender, KeyPressEventArgs e)
        {
            Txt_Size_Text.MaxLength = 4;
            string decimalString = Thread.CurrentThread.CurrentCulture.NumberFormat.CurrencyDecimalSeparator;
            char decimalChar = Convert.ToChar(decimalString);

            if (Char.IsDigit(e.KeyChar) || Char.IsControl(e.KeyChar)) { }
            else if (e.KeyChar == decimalChar && Txt_Size_Text.Text.IndexOf(decimalString) == -1)
            { }
            else
            {
                e.Handled = true;
            }
        }

        private void Chk_Led_CheckedChanged(object sender, EventArgs e)
        {
            /* if (Chk_Led.Checked == true)
             {
                 Lab_Led_On.Enabled = true;
                 Lab_Led_Off.Enabled = true;
                 Com_Led_Off.Enabled = true;
                 Com_Led_On.Enabled = true;
             }
             else
             {
                 Lab_Led_On.Enabled = false;
                 Lab_Led_Off.Enabled = false;
                 Com_Led_Off.Enabled = false;
                 Com_Led_On.Enabled = false;
             }*/
        }

        private void Com_Led_On_DrawItem(object sender, DrawItemEventArgs e)
        {
            Graphics graphics = e.Graphics;
            Rectangle rectangle = e.Bounds;
            if (e.Index >= 0)
            {
                //l?y t麩 m炒
                string namecolor = ((ComboBox)sender).Items[e.Index].ToString();
                //ch?n font ch?
                Font font = new Font("Verdana", 10, FontStyle.Regular);
                //l?y m炒 ch?
                Color color = Color.FromName(namecolor);
                Brush brush = new SolidBrush(color);
                //v? vtm炒 cho kh?i ch? nh?t v?i v? trvk兤h thu?c sau:
                graphics.FillRectangle(brush, rectangle.X - 2, rectangle.Top + 1, 20, 20);
                //hi?n th? t麩 m炒 tuong u?ng v?i m炒 v?a t
                graphics.DrawString(namecolor, font, Brushes.Black, rectangle.X + 20, rectangle.Top + 2);
            }
        }
        //private void Disp_Txt_Paint(object sender, PaintEventArgs e)
        //{
        //Control ctrl = (Control)sender;
        //int ctrl_id;
        //ctrl_id = Disp_return_id_no(ctrl);
        //if (ctrl_id > 1000)
        //{
        //    display_no = ctrl_id / 1000;
        //    ctrl_id = ctrl_id % 1000;

        //}

        //Label lbl= (Label)sender;
        //Graphics graphics = e.Graphics;
        //Rectangle rectangle = lbl.DisplayRectangle;
        //Brush brush = new SolidBrush(Color.FromName(display[display_no].item[ctrl_id].prop.led_off_color));
        //Brush back = new SolidBrush(Color.FromArgb(143,152,152));
        ////v? vtm炒 cho kh?i ch? nh?t v?i v? trvk兤h thu?c sau:
        //graphics.FillRectangle(back, rectangle.X + 2, rectangle.Top + 2, 16, 16);
        //graphics.FillRectangle(brush, rectangle.X + 4, rectangle.Top + 4, 12, 12);

        //}
        private void Com_Led_On_SelectedIndexChanged(object sender, EventArgs e)
        {
            //l?y m炒 tuong ?ng t? t麩 m炒 mta dch?n tr麩 Combobox
            //Color color = Color.FromName(Com_Led_On.Text);

        }

        //private void Txt_Width_Leave(object sender, EventArgs e)
        //{
        //    if (Convert.ToInt32(Txt_Width.Text) < 100 || Convert.ToInt32(Txt_Width.Text) > 5000)
        //    {
        //        MessageBox.Show("Please Width from 100 to 5000 only.");
        //        Txt_Width.Text = "100";
        //        Txt_Width.Focus();
        //        return;
        //    }
        //}

        //private void Txt_Height_Leave(object sender, EventArgs e)
        //{
        //    if (Convert.ToInt32(Txt_Height.Text) < 100 || Convert.ToInt32(Txt_Height.Text) > 5000)
        //    {
        //        MessageBox.Show("Please Width from 100 to 5000 only.");
        //        Txt_Height.Text = "100";
        //        Txt_Height.Focus();
        //        return;
        //    }
        //}

        private void Txt_Size_Text_Leave(object sender, EventArgs e)
        {
            if (Txt_Eng.Text != "" && Txt_Jpn.Text != "")
            {
                if (Txt_Size_Text.Text == "" || float.Parse(Txt_Size_Text.Text) < 1 || float.Parse(Txt_Size_Text.Text) > 1638)
                {
                    MessageBox.Show("Size text from 1 to 1638 only.");
                    Txt_Size_Text.Text = "12";
                    Txt_Size_Text.Focus();
                    return;
                }
            }
        }

        #endregion

        private void Btn_Text_Colors_OnClick(object sender, EventArgs e)
        {
            if (sender == Btn_Text_Color_On)
            {
                ColorDialog colorDialog = new ColorDialog();
                if (colorDialog.ShowDialog() == DialogResult.OK)
                {
                    text_color_on_r = colorDialog.Color.R.ToString();
                    text_color_on_g = colorDialog.Color.G.ToString();
                    text_color_on_b = colorDialog.Color.B.ToString();
                    //prop_enable_disable_onchange_trigger(true);
                    Btn_Text_Color_On.BackColor = colorDialog.Color;
                    if (int.Parse(text_color_on_r) > 127 && int.Parse(text_color_on_g) > 127 && int.Parse(text_color_on_b) > 127)
                        Btn_Text_Color_On.ForeColor = Color.FromArgb(0, 0, 0);
                    else
                        Btn_Text_Color_On.ForeColor = Color.FromArgb(255, 255, 255);
                    //prop_enable_disable_onchange_trigger(false);
                }
            }
            else if (sender == Btn_Text_Color_Off)
            {
                ColorDialog colorDialog = new ColorDialog();
                if (colorDialog.ShowDialog() == DialogResult.OK)
                {
                    text_color_off_r = colorDialog.Color.R.ToString();
                    text_color_off_g = colorDialog.Color.G.ToString();
                    text_color_off_b = colorDialog.Color.B.ToString();
                    Btn_Text_Color_Off.BackColor = colorDialog.Color;
                    if (int.Parse(text_color_off_r) > 127 && int.Parse(text_color_off_g) > 127 && int.Parse(text_color_off_b) > 127)
                        Btn_Text_Color_Off.ForeColor = Color.FromArgb(0, 0, 0);
                    else
                        Btn_Text_Color_Off.ForeColor = Color.FromArgb(255, 255, 255);
                }
            }
            else if (sender == Btn_Text_Color_Press)
            {
                ColorDialog colorDialog = new ColorDialog();
                if (colorDialog.ShowDialog() == DialogResult.OK)
                {
                    text_color_press_r = colorDialog.Color.R.ToString();
                    text_color_press_g = colorDialog.Color.G.ToString();
                    text_color_press_b = colorDialog.Color.B.ToString();
                    Btn_Text_Color_Press.BackColor = colorDialog.Color;
                    if (int.Parse(text_color_press_r) > 127 && int.Parse(text_color_press_g) > 127 && int.Parse(text_color_press_b) > 127)
                        Btn_Text_Color_Press.ForeColor = Color.FromArgb(0, 0, 0);
                    else
                        Btn_Text_Color_Press.ForeColor = Color.FromArgb(255, 255, 255);
                }
            }
        }

        private void Chk_Btn_State_On_CheckedChanged(object sender, EventArgs e)
        {
            if (Chk_Btn_State_On.Checked == true)
            {
                //button_state_check = true;
                Btn_Text_Color_On.Enabled = true;
                Btn_Text_Color_On.BackColor = Color.FromArgb(int.Parse(text_color_on_r), int.Parse(text_color_on_g), int.Parse(text_color_on_b));
                Lab_Image_On.Enabled = true;
                Txt_Image_On.Enabled = true;
                Btn_Open_Image_On.Enabled = true;

            }
            else
            {
                //button_state_check = false;
                Btn_Text_Color_On.Enabled = false;
                Btn_Text_Color_On.BackColor = Color.FromName("White");
                Lab_Image_On.Enabled = false;
                Txt_Image_On.Enabled = false;
                Btn_Open_Image_On.Enabled = false;
            }
        }

        private void Chk_Lab_CheckedChanged(object sender, EventArgs e)
        {
            if (action != CREATE)
            {
                int id = int.Parse(Txt_ID.Text);
                if (display[display_no].type == TYPE_BUTTON)
                    display[display_no].item_button[id].prop.is_label = Chk_Lab.Checked;
                else if (display[display_no].type == TYPE_TEXT)
                    display[display_no].item_textbox[id].prop.is_label = Chk_Lab.Checked;
                //if (action != CREATE)
                //{
                // set is label
                disp_ctrl_create_edit(display_no, id, display[display_no].type);
                //} 
            }
        }

        private void Btn_Create_buttons_MouseDown(object sender, MouseEventArgs e)
        {
            // get dragsize
            Size dragSize = SystemInformation.DragSize;     // $Q: lay gia tri chieu dai va chieu rong cua hinh chu nhat        
            // Create a rectangle using the DragSize, with the mouse position being
            // at the center of the rectangle.
            dragBoxFromMouseDown = new Rectangle(new Point(e.X - (dragSize.Width / 2),
                                                           e.Y - (dragSize.Height / 2)),
                                                 dragSize);           // $Q: Tao hinh chu nhat dua vao dragSize                
            // Press button Button (creating button)
            if (sender == Btn_Button)
            {
                prop_enable_disable_onchange_trigger(true);
                Btnm_Create_button_prepare();
                prop_enable_disable_onchange_trigger(false);
            }
            else if (sender == Btn_Text)
            {
                prop_enable_disable_onchange_trigger(true);
                Btnm_Create_text_prepare();
                prop_enable_disable_onchange_trigger(false);
            }

        }

        private void Btn_Create_buttons_MouseUp(object sender, MouseEventArgs e)
        {
            // Reset the drag rectangle when the mouse button is raised.
            dragBoxFromMouseDown = Rectangle.Empty;
            treeView1.SelectedNode = null;
        }

        private void Btn_Create_buttons_MouseMove(object sender, MouseEventArgs e)
        {
            Button btn = (Button)sender;
            // If the mouse moves outside the rectangle, start the drag.         
            if (dragBoxFromMouseDown != Rectangle.Empty &&
                !dragBoxFromMouseDown.Contains(e.X, e.Y))
            {
                //int id;
                // Proceed with the drag-and-drop, passing in data.                    
                DragDropEffects dropEffect = btn.DoDragDrop(btn, DragDropEffects.All);
                if (dropEffect == DragDropEffects.None)
                {
                    dragBoxFromMouseDown = Rectangle.Empty;
                }
            }
        }

        /*  public string Text_Multiline_return(string input)
          {
              int i;
              string output;
              for (i = 0; i < input.Length; i++)
              {
                  if (input[i] == '\r')
                      output[i] = '\\r';
              }
          }*/
        public string Expo_Return_Init_String(string input)
        {
            string output = "";

            //int count=0;
            int i;
            for (i = 0; i < input.Length; i++)
            {
                if (input[i] == '\\' && input[i + 1] == 'r')
                {
                    output += '\r'; i++;
                }
                else if (input[i] == '\\' && input[i + 1] == 'n')
                {
                    output += '\n'; i++;
                }
                else if (input[i] == '\\' && input[i + 1] == '"')
                {
                    output += '\"'; i++;
                }
                else if (input[i] == '\\' && input[i + 1] == '\\')
                {
                    output += '\\'; i++;
                }
                else
                    output += input[i];
            }
            return output;
            //string output [count];
            //output.Length = input.Length - count;


        }

        private void Com_Led_Off_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void Lab_Led_Click(object sender, EventArgs e)
        {

        }


        private void Txt_Leave(object sender, EventArgs e)
        {

            if (Txt_ID.Text != "")
            {
                Console.WriteLine("**************Txt_Leave****************");
                int ctrl_id = int.Parse(Txt_ID.Text);
                /*           if (ctrl_id > 1000)
                           {
                               display_no = ctrl_id / 1000;
                               ctrl_id = ctrl_id % 1000;

                           }
                 */
                if (display[display_no].type == TYPE_BUTTON)
                {
                    if (Txt_Width.Text == "" || int.Parse(Txt_Width.Text) > display[display_no].size_width || int.Parse(Txt_Width.Text) < 1)
                    {
                        Txt_Width.Text = display[display_no].item_button[ctrl_id].prop.size_width.ToString();
                        Txt_Width.Focus();
                    }
                    if (Txt_Height.Text == "" || int.Parse(Txt_Height.Text) > display[display_no].size_height || int.Parse(Txt_Height.Text) < 1)
                    {
                        //MessageBox.Show("Refill Height");
                        Txt_Height.Text = display[display_no].item_button[ctrl_id].prop.size_height.ToString();
                        Txt_Height.Focus();

                    }
                    if (Txt_X.Text == "" || (int.Parse(Txt_X.Text) + int.Parse(Txt_Width.Text)) > display[display_no].size_width)
                    {
                        // MessageBox.Show("Refill X");
                        Txt_X.Text = display[display_no].item_button[ctrl_id].prop.location_x.ToString();
                        Txt_X.Focus();
                    }
                    if (Txt_Y.Text == "" || (int.Parse(Txt_Y.Text) + int.Parse(Txt_Height.Text)) > display[display_no].size_height)
                    {
                        // MessageBox.Show("Refill Y");
                        Txt_Y.Text = display[display_no].item_button[ctrl_id].prop.location_y.ToString();
                        Txt_Y.Focus();
                    }
                }
                else if (display[display_no].type == TYPE_TEXT)
                {
                    if (Txt_Width.Text == "" || int.Parse(Txt_Width.Text) > display[display_no].size_width || int.Parse(Txt_Width.Text) < 1)
                    {
                        Txt_Width.Text = display[display_no].item_textbox[ctrl_id].prop.size_width.ToString();
                        Txt_Width.Focus();
                    }
                    if (Txt_Height.Text == "" || int.Parse(Txt_Height.Text) > display[display_no].size_height || int.Parse(Txt_Height.Text) < 1)
                    {
                        //MessageBox.Show("Refill Height");
                        Txt_Height.Text = display[display_no].item_textbox[ctrl_id].prop.size_height.ToString();
                        Txt_Height.Focus();

                    }
                    if (Txt_X.Text == "" || (int.Parse(Txt_X.Text) + int.Parse(Txt_Width.Text)) > display[display_no].size_width)
                    {
                        // MessageBox.Show("Refill X");
                        Txt_X.Text = display[display_no].item_textbox[ctrl_id].prop.location_x.ToString();
                        Txt_X.Focus();
                    }
                    if (Txt_Y.Text == "" || (int.Parse(Txt_Y.Text) + int.Parse(Txt_Height.Text)) > display[display_no].size_height)
                    {
                        // MessageBox.Show("Refill Y");
                        Txt_Y.Text = display[display_no].item_textbox[ctrl_id].prop.location_y.ToString();
                        Txt_Y.Focus();
                    }
                }
                else if (display[display_no].type == TYPE_DISPLAY)
                {
                    if (Txt_Width.Text == "" || int.Parse(Txt_Width.Text) > 5000)
                    {
                        //MessageBox.Show("Refill Width");
                        Txt_Width.Text = display[display_no].size_width.ToString();
                        Txt_Width.Focus();


                    }
                    if (Txt_Height.Text == "" || int.Parse(Txt_Height.Text) > 5000)
                    {
                        // MessageBox.Show("Refill Height");
                        Txt_Height.Text = display[display_no].size_height.ToString();
                        Txt_Height.Focus();
                    }
                }
            }
        }

        private void Pos_Txt_ValueChanged(object sender, EventArgs e)
        {
            Pos_Txt.Minimum = 1;
            try
            {
                if (action == EDIT && display[display_no].type != TYPE_DISPLAY && display_no == int.Parse(Txt_display.Text) && Pos_Txt.Value != 0 && flag_pos_max_change == false)
                {
                    int item_id = int.Parse(Txt_ID.Text);
                    int item_id_before = Disp_return_item_id(display_no, Decimal.ToInt32(Pos_Txt.Value));
                    if (display[display_no].type == TYPE_BUTTON)
                    {
                        if (display[display_no].index_button >= item_id_before)
                        {
                            if (display[display_no].item_button[item_id_before].prop.item_pos == Pos_Txt.Value)
                            {
                                int pos_temp = display[display_no].item_button[item_id].prop.item_pos;
                                display[display_no].item_button[item_id].prop.item_pos = display[display_no].item_button[item_id_before].prop.item_pos;
                                display[display_no].item_button[item_id_before].prop.item_pos = pos_temp;
                                Disp_item_pos(display_no);
                            }
                        }
                        if (display[display_no].index_text >= item_id_before)
                        {
                            if (display[display_no].item_textbox[item_id_before].prop.item_pos == Pos_Txt.Value)
                            {
                                int pos_temp = display[display_no].item_button[item_id].prop.item_pos;
                                display[display_no].item_button[item_id].prop.item_pos = display[display_no].item_textbox[item_id_before].prop.item_pos;
                                display[display_no].item_textbox[item_id_before].prop.item_pos = pos_temp;
                                Disp_item_pos(display_no);
                            }
                        }

                    }
                    else if (display[display_no].type == TYPE_TEXT)
                    {
                        if (display[display_no].index_text >= item_id_before)
                        {
                            if (display[display_no].item_textbox[item_id_before].prop.item_pos == Pos_Txt.Value)
                            {
                                int pos_temp = display[display_no].item_textbox[item_id].prop.item_pos;
                                display[display_no].item_textbox[item_id].prop.item_pos = display[display_no].item_textbox[item_id_before].prop.item_pos;
                                display[display_no].item_textbox[item_id_before].prop.item_pos = pos_temp;
                                Disp_item_pos(display_no);
                            }
                        }
                        if (display[display_no].index_button >= item_id_before)
                        {
                            if (display[display_no].item_button[item_id_before].prop.item_pos == Pos_Txt.Value)
                            {
                                int pos_temp = display[display_no].item_textbox[item_id].prop.item_pos;
                                display[display_no].item_textbox[item_id].prop.item_pos = display[display_no].item_button[item_id_before].prop.item_pos;
                                display[display_no].item_button[item_id_before].prop.item_pos = pos_temp;
                                Disp_item_pos(display_no);
                            }
                        }
                    }
                }
                else if (action == EDIT && Pos_Txt.Value == 0 && display_no == int.Parse(Txt_display.Text))

                    Pos_Txt.Value = 0;
            }
            catch { }
        }
        private void Shift_Button(int state)
        {
            int id;
            Control ctrl;

            try
            {
                if (display[display_no].type == TYPE_BUTTON)
                {
                    id = int.Parse(Txt_ID.Text);
                    GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio); // SCALE UP
                    ctrl = (Control)display[display_no].item_button[id].prop.item_disp;
                    ctrl.Location = new Point(display[display_no].item_button[id].prop.location_x, display[display_no].item_button[id].prop.location_y);
                    ctrl.Height = display[display_no].item_button[id].prop.size_height;
                    ctrl.Width = display[display_no].item_button[id].prop.size_width;

                    if (state == UP)
                    {
                        if (shift)
                        {
                            if (ctrl.Location.Y >= 10)
                                display[display_no].item_button[id].prop.location_y -= 10;
                        }
                        else if (ctrl_key)
                        {
                            if (ctrl.Height > 1)
                                display[display_no].item_button[id].prop.size_height -= 1;
                        }
                        else
                        {
                            if (ctrl.Location.Y != 0)
                                display[display_no].item_button[id].prop.location_y--;
                        }
                    }
                    else if (state == DOWN)
                    {
                        if (shift)
                        {
                            if (ctrl.Location.Y + ctrl.Height + 10 <= GroupboxList[display_no].Height)
                                display[display_no].item_button[id].prop.location_y += 10;
                        }
                        else if (ctrl_key)
                        {
                            if (ctrl.Location.Y + ctrl.Height < GroupboxList[display_no].Height)
                                display[display_no].item_button[id].prop.size_height += 1;
                        }
                        else
                        {
                            if (ctrl.Location.Y + ctrl.Height < GroupboxList[display_no].Height)
                            {
                                display[display_no].item_button[id].prop.location_y++;
                            }
                        }
                    }
                    else if (state == LEFT)
                    {
                        if (shift)
                        {
                            if (ctrl.Location.X >= 10)
                                display[display_no].item_button[id].prop.location_x -= 10;
                        }
                        else if (ctrl_key)
                        {
                            if (ctrl.Width > 1)
                                display[display_no].item_button[id].prop.size_width -= 1;
                        }
                        else
                        {
                            if (ctrl.Location.X != 0)
                                display[display_no].item_button[id].prop.location_x--;
                        }

                    }
                    else if (state == RIGHT)
                    {
                        if (shift)
                        {
                            if (ctrl.Location.X + ctrl.Width + 10 <= GroupboxList[display_no].Width)
                                display[display_no].item_button[id].prop.location_x += 10;
                        }
                        else if (ctrl_key)
                        {
                            if (ctrl.Location.X + ctrl.Width < GroupboxList[display_no].Width)
                                display[display_no].item_button[id].prop.size_width += 1;
                        }
                        else
                        {
                            if (ctrl.Location.X + ctrl.Width < GroupboxList[display_no].Width)
                            {
                                display[display_no].item_button[id].prop.location_x++;
                            }
                        }
                    }


                    ctrl.Location = new Point(display[display_no].item_button[id].prop.location_x, display[display_no].item_button[id].prop.location_y);
                    ctrl.Width = display[display_no].item_button[id].prop.size_width;
                    ctrl.Height = display[display_no].item_button[id].prop.size_height;

                    prop_enable_disable_onchange_trigger(true);
                    Txt_X.Text = display[display_no].item_button[id].prop.location_x.ToString();
                    Txt_Y.Text = display[display_no].item_button[id].prop.location_y.ToString();
                    Txt_Height.Text = display[display_no].item_button[id].prop.size_height.ToString();
                    Txt_Width.Text = display[display_no].item_button[id].prop.size_width.ToString();
                    prop_enable_disable_onchange_trigger(false);
                    GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio_recip);

                    Button btn = (Button)ctrl;
                    if (display[disp_id].item_button[id].prop.is_toggle_button && display[display_no].item_button[id].prop.button_state == TOGGLE_BUTTON_STATE.ON)
                    {

                        if (display[disp_id].item_button[id].prop.image_on != null)
                        {
                            try
                            {
                                Image img = Image.FromFile(@display[disp_id].item_button[id].prop.image_on);
                                Bitmap bmp = new Bitmap(img, ctrl.Width, ctrl.Height);
                                btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                            }
                            catch { }
                        }

                    }
                    else
                    {

                        if (display[disp_id].item_button[id].prop.image_off != null)
                        {
                            try
                            {
                                Image img = Image.FromFile(@display[disp_id].item_button[id].prop.image_off);
                                Bitmap bmp = new Bitmap(img, btn.Width, btn.Height);
                                btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                            }
                            catch { }
                        }
                    }

                }
                else if (display[display_no].type == TYPE_TEXT)
                {
                    id = int.Parse(Txt_ID.Text);
                    GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio); // SCALE UP
                    ctrl = (Control)display[display_no].item_textbox[id].prop.item_disp;
                    ctrl.Location = new Point(display[display_no].item_textbox[id].prop.location_x, display[display_no].item_textbox[id].prop.location_y);
                    ctrl.Height = display[display_no].item_textbox[id].prop.size_height;
                    ctrl.Width = display[display_no].item_textbox[id].prop.size_width;

                    if (state == UP)
                    {
                        if (shift)
                        {
                            if (ctrl.Location.Y >= 10)
                                display[display_no].item_textbox[id].prop.location_y -= 10;
                        }
                        else if (ctrl_key)
                        {
                            if (ctrl.Height > 1)
                                display[display_no].item_textbox[id].prop.size_height -= 1;
                        }
                        else
                        {
                            if (ctrl.Location.Y != 0)
                                display[display_no].item_textbox[id].prop.location_y--;
                        }
                    }
                    else if (state == DOWN)
                    {
                        if (shift)
                        {
                            if (ctrl.Location.Y + ctrl.Height + 10 <= GroupboxList[display_no].Height)
                                display[display_no].item_textbox[id].prop.location_y += 10;
                        }
                        else if (ctrl_key)
                        {
                            if (ctrl.Location.Y + ctrl.Height < GroupboxList[display_no].Height)
                                display[display_no].item_textbox[id].prop.size_height += 1;
                        }
                        else
                        {
                            if (ctrl.Location.Y + ctrl.Height < GroupboxList[display_no].Height)
                            {
                                display[display_no].item_textbox[id].prop.location_y++;
                            }
                        }
                    }
                    else if (state == LEFT)
                    {
                        if (shift)
                        {
                            if (ctrl.Location.X >= 10)
                                display[display_no].item_textbox[id].prop.location_x -= 10;
                        }
                        else if (ctrl_key)
                        {
                            if (ctrl.Width > 1)
                                display[display_no].item_textbox[id].prop.size_width -= 1;
                        }
                        else
                        {
                            if (ctrl.Location.X != 0)
                                display[display_no].item_textbox[id].prop.location_x--;
                        }

                    }
                    else if (state == RIGHT)
                    {
                        if (shift)
                        {
                            if (ctrl.Location.X + ctrl.Width + 10 <= GroupboxList[display_no].Width)
                                display[display_no].item_textbox[id].prop.location_x += 10;
                        }
                        else if (ctrl_key)
                        {
                            if (ctrl.Location.X + ctrl.Width < GroupboxList[display_no].Width)
                                display[display_no].item_textbox[id].prop.size_width += 1;
                        }
                        else
                        {
                            if (ctrl.Location.X + ctrl.Width < GroupboxList[display_no].Width)
                            {
                                display[display_no].item_textbox[id].prop.location_x++;
                            }
                        }
                    }


                    ctrl.Location = new Point(display[display_no].item_textbox[id].prop.location_x, display[display_no].item_textbox[id].prop.location_y);
                    ctrl.Width = display[display_no].item_textbox[id].prop.size_width;
                    ctrl.Height = display[display_no].item_textbox[id].prop.size_height;

                    prop_enable_disable_onchange_trigger(true);
                    Txt_X.Text = display[display_no].item_textbox[id].prop.location_x.ToString();
                    Txt_Y.Text = display[display_no].item_textbox[id].prop.location_y.ToString();
                    Txt_Height.Text = display[display_no].item_textbox[id].prop.size_height.ToString();
                    Txt_Width.Text = display[display_no].item_textbox[id].prop.size_width.ToString();
                    prop_enable_disable_onchange_trigger(false);
                    GroupboxList[display_no].Scale(display[display_no].Disp_Scale_ratio_recip);



                }
            }
            catch { }
        }

        private void LSIVN_Hi_Draw_画面作成ツール_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Shift && e.Control)
            {
                shift = false;
                ctrl_key = false;
                state_key = DEFAULT;
                flag_resize = false;
            }
            else if (e.Shift)
            {
                shift = true;
                ctrl_key = false;
                state_key = DEFAULT;

            }
            else if (e.Control)
            {
                ctrl_key = true;
                shift = false;
                state_key = DEFAULT;
            }
            if (flag_resize)
            {
                switch (e.KeyValue)
                {
                    case 37:
                        state_key = LEFT;
                        Shift_Button(state_key);
                        break;
                    case 38:
                        state_key = UP;
                        Shift_Button(state_key);
                        break;
                    case 39:
                        state_key = RIGHT;
                        Shift_Button(state_key);
                        break;
                    case 40:
                        state_key = DOWN;
                        Shift_Button(state_key);
                        break;
                }
            }
        }

        private void LSIVN_Hi_Draw_画面作成ツール_KeyUp(object sender, KeyEventArgs e)
        {

            if (e.KeyValue == 16)
            {
                flag_resize = true;
                shift = false;
                if (e.Control)
                    ctrl_key = true;
            }
            else if (e.KeyValue == 17)
            {
                flag_resize = true;
                ctrl_key = false;
                if (e.Shift)
                    shift = true;
            }

        }

        private void treeView1_KeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = true;
        }

        private void Properties_Enter(object sender, EventArgs e)
        {

        }

        private void Lab_List_Click(object sender, EventArgs e)
        {

        }

    }
}
#region Recycle_Bin
/*private void DynamiclyRsizeAndPositionControls_ResizeBegin(object sender, EventArgs e)
{
    //t匤h chi?u r?ng vcao c?a form tru?c khi cs? ki?n thay d?i k兤h thu?c form
    widthBefore = this.Width;
    heightBefore = this.Height;
}

private void DynamiclyRsizeAndPositionControls_ResizeEnd(object sender, EventArgs e)
{
    //t匤h t? l? gi?a k兤h thu?c sau khi thay d?i vtru?c khi thay d?i cho chi?u cao, r?ng c?a form
    decimal WidthPerscpective = (decimal)Width / widthBefore;
    decimal HeightPerscpective = (decimal)Height / heightBefore;

    //t匤h k兤h thu?c vph穗 b? c當 control cho h?p lv?i k兤h thu?c m?i c?a form
    ResizeAllControls(this, WidthPerscpective, HeightPerscpective);
    widthBefore = this.Width;
    heightBefore = this.Height;
}

private void ResizeAllControls(Control recussiveControl, decimal WidthPerscpective, decimal HeightPerscpective)
{
    foreach (Control control in recussiveControl.Controls)
    {
        //g?i d? quy n?u nhu 1 control n瀰 cch?a c當 control kh當 n?a
        if (control.Controls.Count != 0)
            ResizeAllControls(control, WidthPerscpective, HeightPerscpective);
        //canh l?i to? d? x, y, chi?u r?ng, cao cho c當 control tr麩 form
        control.Left = (int)(control.Left * WidthPerscpective);
        control.Top = (int)(control.Top * HeightPerscpective);
        control.Width = (int)(control.Width * WidthPerscpective);
        control.Height = (int)(control.Height * HeightPerscpective);
    }
}


private void LSIVN_Hi_Draw_画面作成ツール_Resize(object sender, EventArgs e)
{
    if (WindowState != LastWindowState)
    {
        LastWindowState = WindowState;
        if (WindowState == FormWindowState.Maximized)
        {

            decimal WidthPerscpective = (decimal)Width / widthBefore;
            decimal HeightPerscpective = (decimal)Height / heightBefore;
            widthBefore = this.Width;
            heightBefore = this.Height;

            //t匤h k兤h thu?c vph穗 b? c當 control cho h?p lv?i k兤h thu?c m?i c?a form
            ResizeAllControls(this, WidthPerscpective, HeightPerscpective);
        }
        if (WindowState == FormWindowState.Normal)
        {
            decimal WidthPerscpective = (decimal)Width / widthBefore;
            decimal HeightPerscpective = (decimal)Height / heightBefore;
            widthBefore = this.Width;
            heightBefore = this.Height;

            //t匤h k兤h thu?c vph穗 b? c當 control cho h?p lv?i k兤h thu?c m?i c?a form
            ResizeAllControls(this, WidthPerscpective, HeightPerscpective);
        }
    }

}*/


//private void disp_btn(int disp_no, int id)
//{
//    Button btn;
//    // delete ==>flag_state =1 ; set 0
//    if (display[disp_id].flag_state == 1)
//    {
//        btn = (Button)display[disp_id].item[id].prop.item_disp;
//        groupBox1.Controls.Remove(btn);
//        display[disp_id].flag_state = 0;
//    }
//    else
//    {
//        //item[id_no].prop.on_screen = 0;
//        if (display[disp_id].item[id].prop.on_screen == 0)
//        {
//            // tao moi object
//            btn = (Button)new Button();
//            // btn.Size = new Size(item[id_no].prop.size_x, item[id_no].prop.size_y);
//            display[disp_id].item[id].prop.item_disp = btn; // assign
//            display[disp_id].item[id].prop.on_screen = '1';
//            groupBox1.Controls.Add(btn);
//            btn.Name = "button" + id;
//            btn.MouseDown += Disp_btn_Mouse_Down;
//            btn.MouseUp += Disp_btn_Mouse_Up;
//            btn.MouseMove += Disp_ctrl_Mouse_Move;
//            // dragging
//        }
//        btn = (Button)display[disp_id].item[id].prop.item_disp;
//        groupBox1.Scale(Disp_Scale_ratio); // SCALE UP
//        btn.MaximumSize = new Size(display[disp_id].item[id].prop.size_width, display[disp_id].item[id].prop.size_height);
//        btn.MinimumSize = new Size(display[disp_id].item[id].prop.size_width, display[disp_id].item[id].prop.size_height);
//        btn.Location = new Point(display[disp_id].item[id].prop.location_x, display[disp_id].item[id].prop.location_y);
//        btn.Text = display[disp_id].item[id].prop.text_eng;
//        btn.Font = new Font("Arial", display[disp_id].item[id].prop.size_text);  // text size 
//        Image img = Image.FromFile(@display[disp_id].item[id].prop.image_on);
//        Bitmap bmp = new Bitmap(img, btn.Width, btn.Height);
//        btn.Image = Image.FromHbitmap(bmp.GetHbitmap());

//        groupBox1.Scale(Disp_Scale_ratio_recip); // SCALE DOWN
//    }

//}

//private void disp_txt(int id)
//{
//    TextBox txt;
//    //delete
//    if (display[disp_id].flag_state == 1)
//    {
//        txt = (TextBox)display[disp_id].item[id].prop.item_disp;
//        groupBox1.Controls.Remove(txt);
//        display[disp_id].flag_state = 0;
//    }
//    //add
//    else
//    {

//        if (display[disp_id].item[id].prop.on_screen == 0)
//        {
//            txt = new TextBox();
//            display[disp_id].item[id].prop.item_disp = txt;
//            display[disp_id].item[id].prop.on_screen = '1';
//            groupBox1.Controls.Add(txt);
//            txt.Multiline = true;
//           // Helper.ControlMover.Init(txt);
//            txt.Name = "text" + id;

//        }
//        txt = (TextBox)display[disp_id].item[id].prop.item_disp;
//        groupBox1.Scale(Disp_Scale_ratio); // SCALE UP
//        txt.MaximumSize = new Size(display[disp_id].item[id].prop.size_width, display[disp_id].item[id].prop.size_height);
//        txt.MinimumSize = new Size(display[disp_id].item[id].prop.size_width, display[disp_id].item[id].prop.size_height);
//        txt.Location = new Point(display[disp_id].item[id].prop.location_x, display[disp_id].item[id].prop.location_y);
//        txt.BackColor = Color.FromArgb(display[disp_id].item[id].prop.color_r, display[disp_id].item[id].prop.color_g, display[disp_id].item[id].prop.color_b);
//        txt.ForeColor = Color.FromArgb(display[disp_id].item[id].prop.font_color_r, display[disp_id].item[id].prop.font_color_g, display[disp_id].item[id].prop.font_color_b);
//        txt.Font = new Font("Arial", display[disp_id].item[id].prop.size_text);
//        txt.Text = display[disp_id].item[id].prop.text_eng;
//        txt.MouseDown += Disp_txt_Mouse_Down;
//        txt.MouseUp += Disp_txt_Mouse_Up;
//        txt.MouseMove += Disp_ctrl_Mouse_Move;
//        txt.ReadOnly = true;
//        txt.Cursor = Cursors.Default;
//        groupBox1.Scale(Disp_Scale_ratio_recip); // SCALE DOWN
//    }
//}

/*private void disp_preview(int disp_id)
        {
            int id, i;
            int index_btn = 100;
            //Disp_Scale_ratio = new SizeF((float)item[0].prop.size_width / groupBox1.Width, 
            //                        (float)item[0].prop.size_height / groupBox1.Height);
            //Disp_Scale_ratio_recip = new SizeF(1 / Disp_Scale_ratio.Width, 1 / Disp_Scale_ratio.Height);
            Form prv_form = new Form();
            prv_form.AutoScroll = true;
            temp = new GroupBox();
            //GroupBox grp;
            //GroupboxList[disp_id] = (GroupBox)display[disp_id].disp;
            temp.Location = GroupboxList[disp_id].Location;
//            disp_calculate_ratio();
            GroupboxList[disp_id].Scale(Disp_Scale_ratio);

            for (i = 1; i <= display[disp_id].number_button; i++)   // scale button images
            {
                id = index_btn + i;
                if (display[disp_id].item[id].prop.use == 1)
                {
                    try
                    {
                        Button btn;
                        btn = (Button)display[disp_id].item[id].prop.item_disp;
                        Image img = Image.FromFile(@display[disp_id].item[id].prop.image_on);
                        Bitmap bmp = new Bitmap(img, btn.Width, btn.Height);
                        btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                    }
                    catch { }
                }
                else
                {
                    index_btn++;
                    i--;
                }
            }
            GroupboxList[disp_id].Location = new Point(0, 0);
            prv_form.Width = (GroupboxList[disp_id].Width>=600)? GroupboxList[disp_id].Width+50:600;
            prv_form.Height = (GroupboxList[disp_id].Height>=600)? GroupboxList[disp_id].Height+50:600;
            prv_form.MaximumSize = new Size(prv_form.Width, prv_form.Height);
            prv_form.MinimumSize = new Size(prv_form.Width, prv_form.Height);
            //prv_form.WindowState = FormWindowState.Maximized;
            prv_form.MaximizeBox = false;
            //prv_form.MinimizeBox = false; 
            prv_form.Controls.Add(GroupboxList[disp_id]);
            prv_form.Show();
            Allow_drag = false;
            prv_form.FormClosed += new FormClosedEventHandler(disp_prv_form_closed);
            //Allow_drag = false;

        }
  
*/

/*        private void disp_prv_form_closed(object sender, FormClosedEventArgs e)
        {
            //GroupBox grp;
            int i, id;
            int index_btn = 100;
            // GroupboxList[display_no] = (GroupBox)display[display_no].disp;
            //disp_calculate_ratio();
            GroupboxList[display_no].Scale(Disp_Scale_ratio_recip);

            for (i = 1; i <= display[disp_id].number_button; i++)   // scale button images
            {
                id = index_btn + i;
                if (display[disp_id].item[id].prop.use == 1)
                {
                    try
                    {
                        Button btn;
                        btn = (Button)display[disp_id].item[id].prop.item_disp;
                        Image img = Image.FromFile(@display[disp_id].item[id].prop.image_on);
                        Bitmap bmp = new Bitmap(img, btn.Width, btn.Height);
                        btn.Image = Image.FromHbitmap(bmp.GetHbitmap());
                    }
                    catch { }
                }
            }
            GroupboxList[display_no].Location = temp.Location;
            GroupboxList[display_no].Location = new Point(269, 29);
            Allow_drag = true;
            this.Controls.Add(GroupboxList[display_no]);

        }
*/


#endregion









