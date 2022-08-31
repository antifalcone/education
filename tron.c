#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>


char IP[15];
int fb = 0;
int
  right_small = 0,
  right_big = 0,
  left_big = 0,
  left_small = 0,
  up_big = 0,
  up_small = 0,
  down_small = 0,
  down_big = 0;

struct fb_var_screeninfo info;
size_t fb_size, map_size, page_size;
uint32_t *ptr;
int x_window,
    y_window,
    x_centr,
    y_centr;
int work_flag = 1;
struct sockaddr_in me;
struct sockaddr_in other;
int first_fd;
int second_fd;
char direction = '0',key_other = '0';
int size = sizeof(me);
int my_ip[4], alien_ip[4];
int sum_my_ip, sum_alien_ip, big_fail, small_fail;
uint32_t *ptr;

void bort() {
      int i = 1,k;
      uint32_t color_0 = 0xffffffff;
      mvprintw(30,30,"Enter enter ");
      scanw("%d", &k);
      mvprintw(30,30,"               ");
      refresh();
      move(1,COLS-10);
      refresh();
      ptr[(y_centr - (y_window/2)) * info.xres_virtual + (x_centr - (x_window/2))] = color_0;
      while(i < x_window) {
        ptr[(y_centr - y_window/2) * info.xres_virtual + (x_centr - x_window/2) + i] = color_0;
        i++;
      }
      i = 1;
      while(i < y_window) {
        ptr[(y_centr - (y_window/2) + i) * info.xres_virtual + (x_centr + (x_window/2))] = color_0;
        i++;
      }
      i = 1;
      while(i < x_window) {
        ptr[(y_centr + y_window/2) * info.xres_virtual + (x_centr + x_window/2) - i] = color_0;
        i++;
      }
      i = 1;
      while(i < y_window) {
        ptr[(y_centr + y_window/2 - i) * info.xres_virtual + (x_centr - x_window/2)] = color_0;
        i++;
      }
      i = 0;
      sendto(first_fd,&k,1,MSG_CONFIRM,(struct sockaddr*)&other,size);
      while(i == 0){
        if(key_other != '0') i++;
      }
      right_big = 1;
      left_small = 1;
      }


void handler(int none) {
    work_flag = 0;
  }

int compare(char* ip_other) {
  sscanf(IP,"%d.%d.%d.%d",&my_ip[0],&my_ip[1],&my_ip[2],&my_ip[3]);
  sscanf(ip_other,"%d.%d.%d.%d",&alien_ip[0],&alien_ip[1],&alien_ip[2],&alien_ip[3]);
  sum_my_ip = my_ip[0] + my_ip[1] + my_ip[2] + my_ip[3];
  sum_alien_ip = alien_ip[0] + alien_ip[1] + alien_ip[2] + alien_ip[3];
}

void go_small() {
  int counter,i,flag_for_death = 0;
  int xstep = 1, ystep = 1;
  int previous_up_for_small, previous_left_for_small, previous_down_for_small, previous_right_for_small;
  uint32_t color_0 = 0x00000fff;
  //int x_window = x_window, y_window = y_window,
  int y_center = y_centr, x_center = x_centr;
  int x = x_window - 9;
  int y = y_window - 6;
  while(counter < 2) {
    int a = left_small, w = up_small, s = down_small, d = right_small;
    if(d == 1) {
      i = 0;
      if(previous_right_for_small == 0){
	if(previous_up_for_small == 1) {
    while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
      	    i++;
	  }
	}

	if(previous_down_for_small == 1) {
	  while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
      	    i++;
	  }
	}
	i = 0;
	x += 9;
	ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x - 8] = color_0;
  while(i < 8) {
    if (ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] != 0 ||
    ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] != 0 ||
    ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] != 0 ||
    ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] != 0 ||
    ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] != 0 )
    {
      flag_for_death = 1;
      break;
    }
    ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] = color_0;
    ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] = color_0;
    ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] = color_0;
    ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] = color_0;
    ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] = color_0;
    i++;
    previous_right_for_small = 1;
previous_up_for_small = 0;
previous_down_for_small = 0;
previous_left_for_small = 0;
  }
  if (flag_for_death == 1) {
    small_fail = 1;
    break;
  }
}
if (ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 )
{
  //flag_for_death = 1;
  small_fail;
  break;
}

  ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
  ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
  ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
  ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;

  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7] = 0;
  ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7] = 0;
  ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7] = 0;
  ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7] = 0;
  /*if (flag_for_death == 1) {
    big_fail = 1;
    break;
  }*/
        x++;

    }
    if(s == 1) {
      i = 0;
      if(previous_down_for_small == 0){
	if(previous_left_for_small == 1) {
	  while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	    ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	    ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	    ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	    i++;
	  }
	}
	if(previous_right_for_small == 1) {
	  while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    i++;
	  }
	}
	i = 0;
	y += 9;
	ptr[(y_center - (y_window/2) + y - 8) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
  while(i < 8) {
    if(ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x] != 0 ||
    ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
    ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] != 0 ||
    ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
    ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] != 0)
    {
      flag_for_death = 1;
      break;
    }

    ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
    ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
    ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = color_0;
    ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
    ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = color_0;
    i++;
    previous_right_for_small = 0;
previous_up_for_small = 0;
previous_left_for_small = 0;
previous_down_for_small = 1;
}
if (flag_for_death == 1) {
  small_fail = 1;
break;
  }
}
if(ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x] != 0 ||
ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 2* xstep] != 0 ||
ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] != 0 ||
ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x] != 0)
{
  //flag_for_death = 1;
  small_fail = 1;
  break;
}
  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 2* xstep] = color_0;
  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = color_0;
ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
  ptr[(y_center - (y_window/2) + y - 7) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
  ptr[(y_center - (y_window/2) + y - 7) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
  ptr[(y_center - (y_window/2) + y - 7) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
  ptr[(y_center - (y_window/2) + y - 7) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;


  /*if (flag_for_death == 1) {
    big_fail = 1;
  break;
}*/
      y++;
    }
    if(a == 1) {
      i = 0;
      if(previous_left_for_small == 0){
        if(previous_up_for_small == 1) {
	    while(i < 8) {
	      ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
      	      ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;
      	      ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
      	      ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
      	      i++;
	    }
	  }
	if(previous_down_for_small == 1) {
	  while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
      	    i++;
	  }
	}
	i = 0;
	x -= 9;
	ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + 8] = color_0;
  while(i < 8) {
    if(ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] != 0 ||
    ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] != 0 ||
    ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] != 0 ||
    ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] != 0 ||
    ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] != 0 )
    {
      flag_for_death = 1;
      break;
    }
    ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] = color_0;
    ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] = color_0;
    ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] = color_0;
    ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] = color_0;
    ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] = color_0;
    i++;
    previous_right_for_small = 0;
previous_up_for_small = 0;
previous_left_for_small = 1;
previous_down_for_small = 0;
}
if (flag_for_death == 1) {
small_fail = 1;
break;
}

  }
if(ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 )
{
  //flag_for_death = 1;
  small_fail = 1;
  break;
}
  ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
  ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
  ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
  ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;

  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7] = 0;
  ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7] = 0;
  ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7] = 0;
  ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7] = 0;
  /*if (flag_for_death == 1) {
    big_fail = 1;
    break;
  }*/
        x--;
    }
    if(w == 1) {
      int i = 0;
      if(previous_up_for_small == 0){
	if(previous_left_for_small == 1) {
	   while(i < 8) {
	     ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	     ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	     ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	     ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	     i++;
	   }
	}
	if(previous_right_for_small == 1) {
	  while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    i++;
	  }
	}
	i = 0;
  y -= 9;
	ptr[(y_center - (y_window/2) + y + 8) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
  while(i < 8) {

    if( ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x] != 0 ||
    ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
    ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] != 0 ||
    ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
    ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] != 0 )
    {
      flag_for_death = 1;
      break;
    }
    ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
    ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
    ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = color_0;
    ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
    ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = color_0;
    i++;
    previous_right_for_small = 0;
previous_up_for_small = 1;
previous_left_for_small = 0;
previous_down_for_small = 0;
}
if (flag_for_death == 1) {
small_fail = 1;
break;
  }
}
if(
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x] != 0 ||
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 2* xstep] != 0 ||
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] != 0 )
{
  //flag_for_death = 1;
  small_fail = 1;
  break;
}
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 2* xstep] = color_0;
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = color_0;

ptr[(y_center - (y_window/2) + y + 7) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
ptr[(y_center - (y_window/2) + y + 7) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
ptr[(y_center - (y_window/2) + y + 7) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
ptr[(y_center - (y_window/2) + y + 7) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;
/*if (flag_for_death == 1) {
  big_fail = 1;
  break;
}*/
      y--;

  }
  usleep(64500);
 if (big_fail != 0) counter++;
  }
}




void keyboard() {
 int counter;
  if(sum_my_ip > sum_alien_ip) {
    while(counter<2) {
      direction = getch();
      switch(direction) {
        case 'w':
	 if(down_big == 1) break;
          left_big = down_big = right_big = 0;
          up_big = 1;
	  sendto(first_fd,&direction,1,MSG_CONFIRM,(struct sockaddr*)&other,size);
          break;
        case 'a':
	  if(right_big == 1)break;
          up_big = down_big = right_big = 0;
          left_big = 1;
          sendto(first_fd,&direction,1,MSG_CONFIRM,(struct sockaddr*)&other,size);
          break;
        case 's':
          if(up_big == 1) break;
          up_big = left_big = right_big = 0;
          down_big = 1;
          sendto(first_fd,&direction,1,MSG_CONFIRM,(struct sockaddr*)&other,size);
          break;
        case 'd':
	  if(left_big == 1) break;
          up_big = left_big = down_big = 0;
          right_big = 1;
          sendto(first_fd,&direction,1,MSG_CONFIRM,(struct sockaddr*)&other,size);
          break;
      }
	if (big_fail != 0 || small_fail != 0) counter++;
    }
  }
  if(sum_my_ip < sum_alien_ip) {
    while(counter<2) {
      direction = getch();
      switch(direction) {
        case 'w':
	  if(down_small == 1) break;
          left_small = down_small = right_small = 0;
          up_small = 1;
	  sendto(first_fd,&direction,1,MSG_CONFIRM,(struct sockaddr*)&other,size);
          break;
        case 'a':
	  if(right_small == 1) break;
          up_small = down_small = right_small = 0;
          left_small = 1;
          sendto(first_fd,&direction,1,MSG_CONFIRM,(struct sockaddr*)&other,size);
          break;
        case 's':
 	  if(up_small == 1) break;
          up_small = left_small = right_small = 0;
          down_small = 1;
          sendto(first_fd,&direction,1,MSG_CONFIRM,(struct sockaddr*)&other,size);
          break;
        case 'd':
 	  if (left_small == 1) break;
          up_small = left_small = down_small = 0;
          right_small = 1;
          sendto(first_fd,&direction,1,MSG_CONFIRM,(struct sockaddr*)&other,size);
          break;
      }
	if (big_fail != 0 || small_fail != 0) counter++;
    }
  }
}


void* recw(){
  int counter;
  if(sum_my_ip < sum_alien_ip) {
    while(counter<2){
      recvfrom(first_fd,&key_other,1,MSG_WAITALL,(struct sockaddr*)&me,&size);
      switch(key_other) {
        case 'w':
	       if(down_big == 1) break;
         up_big = 1;
         left_big = 0;
         down_big = 0;
         right_big = 0;
	       break;
        case 's':
	       if(up_big == 1) break;
         down_big = 1;
         up_big = 0;
         right_big = 0;
         left_big = 0;
         break;
        case 'a':
  	     if(right_big == 1) break;
         left_big = 1;
         up_big = 0;
         down_big = 0;
         right_big = 0;
         break;
        case 'd':
	       if(left_big == 1) break;
         right_big = 1;
         up_big = 0;
         left_big = 0;
         down_big = 0;
         break;
      }
	if (big_fail != 0 || small_fail != 0) counter++;
    }
  }
  if(sum_my_ip > sum_alien_ip) {
    while(counter<2){
                  recvfrom(first_fd,&key_other,1,MSG_WAITALL,(struct sockaddr*)&me,&size);
                  switch(key_other) {
                            case 's':
	                           if (up_small == 1) break;
                             up_small = 0;
                             left_small = 0;
                             right_small = 0;
                             down_small = 1;
                             break;
                            case 'w':
	                           if (down_small == 1) break;
                             up_small = 1;
                             left_small = 0;
                             down_small = 0;
                             right_small = 0;
                             break;
                            case 'a':
	                           if (right_small == 1) break;
                             left_small = 1;
                             up_small = 0;
                             down_small = 0;
                             right_small = 0;
                             break;
                            case 'd':
	                           if (left_small == 1) break;
                             right_small = 1;
                             up_small = 0;
                             left_small = 0;
                             down_small = 0;
                             break;
      }
	if (big_fail != 0 || small_fail != 0) counter++;
    }
  }
}



void go_big() {
  //int x_window = x_window, y_window = y_window,
  int y_center = y_centr, x_center = x_centr;
  int counter,i,flag_for_death = 0;
  int x = 9, y = 5, xstep = 1, ystep = 1;
  int previous_up_for_big, previous_left_for_big, previous_down_for_big, previous_right_for_big;
  uint32_t color_0 = 0xfff00000;


  while(counter<2) {
    int a = left_big, w = up_big, s = down_big, d = right_big;
    if(d == 1) {
      i = 0;
      if(previous_right_for_big == 0){
	if(previous_up_for_big == 1) {
    while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
      	    i++;
	  }
	}

	if(previous_down_for_big == 1) {
	  while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
      	    i++;
	  }
	}
	i = 0;
	x += 9;
	ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x - 8] = color_0;
      	while(i < 8) {
          if (ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] != 0 ||
          ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] != 0 ||
          ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] != 0 ||
          ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] != 0 ||
          ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] != 0 )
          {
            flag_for_death = 1;
            break;
          }
      	  ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] = color_0;
      	  ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] = color_0;
      	  ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] = color_0;
      	  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] = color_0;
      	  ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7 + i] = color_0;
      	  i++;
      	  previous_right_for_big = 1;
	  previous_up_for_big = 0;
	  previous_down_for_big = 0;
	  previous_left_for_big = 0;
      	}
        if (flag_for_death == 1) {
          big_fail = 1;
          break;
        }
      }
      if (ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 )
      {
        //flag_for_death = 1;
	big_fail = 1;
        break;
      }

        ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
        ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
        ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
        ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
        ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;

        ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7] = 0;
        ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7] = 0;
        ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7] = 0;
        ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 7] = 0;
        /*if (flag_for_death == 1) {
          big_fail = 1;
          break;
        }*/
        x++;

    }
    if(s == 1) {
      i = 0;
      if(previous_down_for_big == 0){
	if(previous_left_for_big == 1) {
	  while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	    ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	    ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	    ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	    i++;
	  }
	}
	if(previous_right_for_big == 1) {
	  while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    i++;
	  }
	}
	i = 0;
	y += 9;
	ptr[(y_center - (y_window/2) + y - 8) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
      	while(i < 8) {
          if(ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x] != 0 ||
          ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
          ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] != 0 ||
          ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
          ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] != 0)
          {
            flag_for_death = 1;
            break;
          }

      	  ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
      	  ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
      	  ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = color_0;
      	  ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
      	  ptr[(y_center - (y_window/2) + y - 7 + i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = color_0;
      	  i++;
      	  previous_right_for_big = 0;
	  previous_up_for_big = 0;
	  previous_left_for_big = 0;
	  previous_down_for_big = 1;
  }
      if (flag_for_death == 1) {
        big_fail = 1;
      break;
      	}
      }
      if(ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x] != 0 ||
      ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 2* xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x] != 0)
      {
        //flag_for_death = 1;
        big_fail = 1;
	break;
      }
        ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
        ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 2* xstep] = color_0;
        ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
        ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = color_0;
	ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
        ptr[(y_center - (y_window/2) + y - 7) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
        ptr[(y_center - (y_window/2) + y - 7) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
        ptr[(y_center - (y_window/2) + y - 7) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
        ptr[(y_center - (y_window/2) + y - 7) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;


        /*if (flag_for_death == 1) {
          big_fail = 1;
        break;
      }*/
    y++;
  }
    if(a == 1) {
      i = 0;
      if(previous_left_for_big == 0){
        if(previous_up_for_big == 1) {
	    while(i < 8) {
	      ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
      	      ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;
      	      ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
      	      ptr[(y_center - (y_window/2) + y + i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
      	      i++;
	    }
	  }
	if(previous_down_for_big == 1) {
	  while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
      	    ptr[(y_center - (y_window/2) + y - i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
      	    i++;
	  }
	}
	i = 0;
	x -= 9;
	ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + 8] = color_0;
      	while(i < 8) {
          if(ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] != 0 ||
      	  ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] != 0 ||
      	  ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] != 0 ||
      	  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] != 0 ||
      	  ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] != 0 )
          {
            flag_for_death = 1;
            break;
          }
      	  ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] = color_0;
      	  ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] = color_0;
      	  ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] = color_0;
      	  ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] = color_0;
      	  ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7 - i] = color_0;
      	  i++;
      	  previous_right_for_big = 0;
	  previous_up_for_big = 0;
	  previous_left_for_big = 1;
	  previous_down_for_big = 0;
  }
    if (flag_for_death == 1) {
      big_fail = 1;
      break;
    }

      	}
      if(ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 )
      {
        //flag_for_death = 1;
	big_fail = 1;
        break;
      }
        ptr[(y_center - (y_window/2) + y) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
        ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
        ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
        ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
        ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;

        ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7] = 0;
        ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7] = 0;
        ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7] = 0;
        ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 7] = 0;
        /*if (flag_for_death == 1) {
          big_fail = 1;
          break;
        }*/
        x--;
    }
    if(w == 1) {
      int i = 0;
      if(previous_up_for_big == 0){
	if(previous_left_for_big == 1) {
	   while(i < 8) {
	     ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	     ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	     ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	     ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + i] = 0;
      	     i++;
	   }
	}
	if(previous_right_for_big == 1) {
	  while(i < 8) {
      	    ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y - 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y + ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    ptr[(y_center - (y_window/2) + y + 2*ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - i] = 0;
      	    i++;
	  }
	}
	i = 0;
  y -= 9;
	ptr[(y_center - (y_window/2) + y + 8) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
      	while(i < 8) {

          if( ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x] != 0 ||
      	  ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
      	  ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] != 0 ||
      	  ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
      	  ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] != 0 )
          {
            flag_for_death = 1;
            break;
          }
      	  ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
      	  ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
      	  ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = color_0;
      	  ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
      	  ptr[(y_center - (y_window/2) + y + 7 - i) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = color_0;
      	  i++;
      	  previous_right_for_big = 0;
	  previous_up_for_big = 1;
	  previous_left_for_big = 0;
	  previous_down_for_big = 0;
  }
    if (flag_for_death == 1) {
      big_fail = 1;
      break;
      	}
      }
      if(
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x] != 0 ||
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 2* xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] != 0 ||
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] != 0 )
      {
        //flag_for_death = 1;
	big_fail = 1;
        break;
      }
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x] = color_0;
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = color_0;
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x - 2* xstep] = color_0;
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = color_0;
      ptr[(y_center - (y_window/2) + y - ystep) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = color_0;

      ptr[(y_center - (y_window/2) + y + 7) * info.xres_virtual + (x_center - (x_window/2)) + x + xstep] = 0;
      ptr[(y_center - (y_window/2) + y + 7) * info.xres_virtual + (x_center - (x_window/2)) + x + 2*xstep] = 0;
      ptr[(y_center - (y_window/2) + y + 7) * info.xres_virtual + (x_center - (x_window/2)) + x - xstep] = 0;
      ptr[(y_center - (y_window/2) + y + 7) * info.xres_virtual + (x_center - (x_window/2)) + x - 2*xstep] = 0;
      /*if (flag_for_death == 1) {
        big_fail = 1;
        break;
      }*/
      y--;

  }
  usleep(64500);
  if (small_fail != 0) counter++;
  }
}








int main(int argc, char *argv[]) {
  uint32_t color_0 = 0xffffffff;
  int i;
        signal(SIGINT, handler);
  page_size = sysconf(_SC_PAGESIZE);

  if ( 0 > (fb = open("/dev/fb0", O_RDWR))) {
    perror("open");
    return __LINE__;
  }

  if ( (-1) == ioctl(fb, FBIOGET_VSCREENINFO, &info)) {
    perror("ioctl");
    close(fb);
    return __LINE__;
  }

  fb_size = sizeof(uint32_t) * info.xres_virtual * info.yres_virtual;
  map_size = (fb_size + (page_size - 1 )) & (~(page_size-1));

  ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

  if ( MAP_FAILED == ptr ) {
    perror("mmap");
    close(fb);
    return __LINE__;
  }

  if(NULL == initscr())
    return __LINE__;

  if(argc<3){
    printf("Usage: %s Your_ip enemy_ip\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  if ( (first_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  me.sin_family  = AF_INET;
  me.sin_addr.s_addr = inet_addr(IP);
  me.sin_port = htons(12345);
  if ( bind(first_fd, (const struct sockaddr *)&me,sizeof(me)) < 0 ) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if ( (second_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  other.sin_family = AF_INET;
  other.sin_addr.s_addr = inet_addr((argv[3]));
  other.sin_port = htons(12345);
  //char str[256];
  //FILE* stream = popen(system("ls"),"w");
  //fscanf(stream, "s", str);
  //fprintf(str);
  //while ( str[s]
  //compare(ip_other);
  FILE* stream = popen("ip route get 1.2.3.4| awk '{print$7}'","w");
  fgets(IP,15,stream);
  //стоит поменять, т.к. эта штука только для 1920х1080
	x_window = atoi(argv[1]) - 1;
      if(x_window > 1919)
        x_window = 1919;
      y_window = atoi(argv[2]) - 1;
      if(y_window > 1079)
        y_window = 1079;
      y_centr = info.yres_virtual/2;
      x_centr = info.xres_virtual/2;

  sscanf(IP,"%d.%d.%d.%d",&my_ip[0],&my_ip[1],&my_ip[2],&my_ip[3]);
  sscanf(argv[3],"%d.%d.%d.%d",&alien_ip[0],&alien_ip[1],&alien_ip[2],&alien_ip[3]);
  sum_my_ip = my_ip[0] + my_ip[1] + my_ip[2] + my_ip[3];
  sum_alien_ip = alien_ip[0] + alien_ip[1] + alien_ip[2] + alien_ip[3];
  pthread_t sock;
  pthread_create(&sock, NULL, recw, NULL);

  noecho();
  keypad(stdscr, TRUE);

  bort();
  pthread_t claw;
  pthread_create(&claw, NULL, keyboard, NULL);
  pthread_t big;
  pthread_create(&big, NULL, go_big, NULL);
  pthread_t small;
  pthread_create(&small, NULL, go_small, NULL);

  while(big_fail == 0 && small_fail == 0) sleep(1);
  left_big = 0,
  up_big = 0,
  down_big = 0,
  right_big = 0,
  left_small = 0,
  down_small = 0,
  right_small = 0,
  up_small = 0;
  if(big_fail == 1 && small_fail == 0) {
    printw("Blue win!");
    refresh();
    sleep(3);
  }
  if(big_fail == 0 && small_fail == 1) {
    printw("Red win!");
    refresh();
    sleep(3);
  }
  if(big_fail == 1 && small_fail == 1) {
    printw("Both lost!");
    refresh();
    sleep(3);
  }
  munmap(ptr, map_size);
  close(fb);
  endwin();
  system("reset");
  exit(1);
  }
