#include <gtest/gtest.h>

#include "staffLineGrid.h"

TEST(StaffLineGridTest, addNote) {
    StaffLineGrid grid;

    grid.AddNote(0,0);
    EXPECT_TRUE(grid.HasNote(0,0));
}

TEST(StaffLineGridTest, addNote_OutOfBounds) {
    StaffLineGrid grid;

    grid.AddNote(-1,-1);
    EXPECT_FALSE(grid.HasNote(-1,-1));
}

TEST(StaffLineGridTest, addNote_Locked) {
    StaffLineGrid grid;

    grid.LockGrid();
    grid.AddNote(0,0);
    EXPECT_FALSE(grid.HasNote(0,0));
}

TEST(StaffLineGridTest, addNote_Length) {
    StaffLineGrid grid;

    grid.AddNote(0,0,2);
    EXPECT_TRUE(grid.HasNote(0,0));
    EXPECT_TRUE(grid.HasNote(1,0));
}

TEST(StaffLineGridTest, addNote_Length_OutOfBounds_Left) {
    StaffLineGrid grid;

    grid.AddNote(-1,0,2);
    EXPECT_FALSE(grid.HasNote(-1,0));
    EXPECT_FALSE(grid.HasNote(0,0));
}

TEST(StaffLineGridTest, addNote_Length_OutOfBounds_Right) {
    StaffLineGrid grid;

    grid.AddNote(grid.columns,0,2);
    EXPECT_FALSE(grid.HasNote(grid.columns,0));
    EXPECT_FALSE(grid.HasNote(grid.columns+1,0));
}

TEST(StaffLineGridTest, addNote_Length_Locked) {
    StaffLineGrid grid;

    grid.LockGrid();
    grid.AddNote(0,0,2);
    EXPECT_FALSE(grid.HasNote(0,0));
    EXPECT_FALSE(grid.HasNote(1,0));
}

TEST(StaffLineGridTest, removeNote) {
    StaffLineGrid grid;

    grid.AddNote(0,0);
    EXPECT_TRUE(grid.HasNote(0,0));
    grid.RemoveNote(0,0);
    EXPECT_FALSE(grid.HasNote(0,0));
}

TEST(StaffLineGridTest, removeNote_Locked) {
    StaffLineGrid grid;

    grid.AddNote(0,0);
    EXPECT_TRUE(grid.HasNote(0,0));
    grid.LockGrid();
    grid.RemoveNote(0,0);
    EXPECT_TRUE(grid.HasNote(0,0));
}

TEST(StaffLineGridTest, removeNote_Length) {
    StaffLineGrid grid;

    grid.AddNote(0,0,3);
    EXPECT_TRUE(grid.HasNote(0,0));
    EXPECT_TRUE(grid.HasNote(1,0));
    EXPECT_TRUE(grid.HasNote(2,0));
    grid.RemoveNote(1,0);
    EXPECT_FALSE(grid.HasNote(0,0));
    EXPECT_FALSE(grid.HasNote(1,0));
    EXPECT_FALSE(grid.HasNote(2,0));
}

TEST(StaffLineGridTest, ClearGrid) {
    StaffLineGrid grid;

    grid.AddNote(0,0,2);
    EXPECT_TRUE(grid.HasNote(0,0));
    EXPECT_TRUE(grid.HasNote(1,0));

    grid.AddNote(3,4);
    EXPECT_TRUE(grid.HasNote(3,4));

    grid.AddNote(2,1);
    EXPECT_TRUE(grid.HasNote(2,1));

    grid.ClearGrid();

    EXPECT_FALSE(grid.HasNote(0,0));
    EXPECT_FALSE(grid.HasNote(1,0));
    EXPECT_FALSE(grid.HasNote(3,4));
    EXPECT_FALSE(grid.HasNote(2,1));
}

TEST(StaffLineGridTest, LockGrid) {
    StaffLineGrid grid;

    grid.LockGrid();
    grid.AddNote(0,0);
    EXPECT_FALSE(grid.HasNote(0,0));
}

TEST(StaffLineGridTest, UnlockGrid) {
    StaffLineGrid grid;

    grid.LockGrid();
    grid.LockGrid(); // Test if toggle
    grid.AddNote(0,0);
    EXPECT_FALSE(grid.HasNote(0,0));
    grid.UnlockGrid();
    grid.UnlockGrid(); // Test if toggle
    grid.AddNote(0,0);
    EXPECT_TRUE(grid.HasNote(0,0));
}

TEST(StaffLineGridTest, GetLockedStatus) {
    StaffLineGrid grid;

    grid.LockGrid();
    EXPECT_TRUE(grid.GetLockedStatus());
    grid.UnlockGrid();
    EXPECT_FALSE(grid.GetLockedStatus());
}